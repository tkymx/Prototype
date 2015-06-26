//---------------------------------------------------------------------------------------------------------------------
//グローバル変数
//---------------------------------------------------------------------------------------------------------------------

//ビューと射影空間への座標変換
float4x4 g_mVP;
//ローカル座標から射影空間への座標変換
float4x4 g_mWVP;
		
//ボーン用変換行列の配列
static const int MAX_MATRICES = 26;
float4x3 g_mWorldMatrixArray[MAX_MATRICES];
//ボーン数(0のときは通常レンダリング)
int g_NumBones=2;

//光源の設定(法線考慮の陰影計算用光源ベクトル)
float3 g_vLight;

//カメラの位置(通常描画用(太陽として))
float3 g_vEyePos;

//光の強さ(ユーザ制御)
float4 g_vLightAmb = { 1.0f, 1.0f, 1.0f, 1.0f };
float4 g_vLightDif = { 1.0f, 1.0f, 1.0f, 1.0f };

//実際のライティング時の強さ
float4 g_vAmb = { 0.1f, 0.1f, 0.1f, 1.0f };
float4 g_vDif = { 0.8f, 0.8f, 0.8f, 1.0f };

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//テクスチャ
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

//XFILEのテクスチャ
texture g_Tex;
sampler Samp = sampler_state
{

	Texture = <g_Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;

};


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//頂点シェーダからピクセルシェーダに渡す構造体
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct VS_DATA
{
	float4 m_Pos			:POSITION0;		//頂点データ
	float2 m_Tex			:TEXCOORD0_centroid;		//テクスチャデータ
	
	float4 m_Color			:COLOR0;		//カラー	

	float3 m_Normal			:TEXCOORD1;		//法線ベクトル
	float3 m_Eye				:TEXCOORD2;		//視点ベクトル

};

struct VS_DATA_SHADOW
{
	float4 m_Pos			:POSITION0;		//頂点データ
	float2 m_Tex			:TEXCOORD0_centroid;		//テクスチャデータ
	float4 m_WorldPos		:TEXCOORD7;		//ワールド変換後の頂点データ
	
	float4 m_Ambient		:COLOR0;		//環境光			
	float4 m_Diffuse		:COLOR1;		//拡散光

	float3 m_Normal			:TEXCOORD1;		//法線ベクトル
	float3 m_Eye			:TEXCOORD2;		//視点ベクトル
	
	float4 m_ShadowMapUV	:TEXCOORD3;		//シャドウUV座標
	float4 m_Depth			:TEXCOORD4;		//深度比較用	
	float4 m_Shadow			:TEXCOORD5;		//影比較用

	float3 m_LightEye		:TEXCOORD6;		//ライトの視点の位置ベクトル(座標ピクセルから)

};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//補助プログラム　
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------
//ボーンを適応
//--------------
VS_DATA ApplyBoon(
	float4 a_Pos					:POSITION,			//頂点データ	
	float3 a_Normal				:NORMAL,				//法線データ
	float4  a_BlendWeights   : BLENDWEIGHT,	//ブレンディングの重み
    float4  a_BlendIndices    : BLENDINDICES,	//ブレンドのインデックス
	uniform int NumBones								//ボーン数
){

		VS_DATA l_out;
		float3      Pos = 0.0f;
		float3      Normal = 0.0f;    
		float       LastWeight = 0.0f;
     
		// Compensate for lack of UBYTE4 on Geforce3
		int4 IndexVector = D3DCOLORtoUBYTE4(a_BlendIndices);

		// cast the vectors to arrays for use in the for loop below
		float BlendWeightsArray[4] = (float[4])a_BlendWeights;
		int   IndexArray[4]        = (int[4])IndexVector;
    
		// calculate the pos/normal using the "normal" weights 
		//        and accumulate the weights to calculate the last weight
		for (int iBone = 0; iBone < NumBones-1; iBone++)
		{
			LastWeight = LastWeight + BlendWeightsArray[iBone];
        
			Pos += mul(a_Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
			Normal += mul(a_Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
		}
		LastWeight = 1.0f - LastWeight; 

		// Now that we have the calculated weight, add in the final influence
		Pos += (mul(a_Pos, g_mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);
		Normal += (mul(a_Normal, g_mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); 

		// normalize normals
		Normal = normalize(Normal);

		l_out.m_Pos = float4(Pos.xyz, 1.0f);
		l_out.m_Normal = Normal;

	return l_out;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//頂点シェーダプログラム　
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------
//テクスチャ表示
//--------------
VS_DATA VS_TEXTURE(
	float4 a_Pos					:POSITION,			//頂点データ	
	float2 a_Tex					:TEXCOORD,			//テクスチャデータ
	float4 a_Color					:COLOR		//頂点データ	
){

	VS_DATA l_Out	= (VS_DATA)0;			//出力データ

	l_Out.m_Pos = a_Pos = mul( a_Pos , g_mWVP );	//位置座標 ;

	l_Out.m_Tex = a_Tex;				//テクスチャ座標

	l_Out.m_Color =	  g_vAmb + g_vDif ;

	//カラー計算
	l_Out.m_Color = l_Out.m_Color * a_Color;

	return l_Out; 

}
//--------------
//テクスチャ表示光あり
//--------------
VS_DATA VS(
	float4 a_Pos					:POSITION,			//頂点データ	
	float2 a_Tex					:TEXCOORD,			//テクスチャデータ
	float3 a_Normal				:NORMAL,				//法線データ
	float4 a_Color					:COLOR,			//頂点データ	
	float4  a_BlendWeights   : BLENDWEIGHT,	//ブレンディングの重み
    float4  a_BlendIndices    : BLENDINDICES,	//ブレンドのインデックス
	uniform int a_NumBones								//ボーン数
){

	VS_DATA l_Out	= (VS_DATA)0;			//出力データ
	VS_DATA l_boonapply = (VS_DATA)0;

	//ボーン情報があったら
	if(g_NumBones != 0)
	{
		//ボーンの適用
		l_boonapply = ApplyBoon( a_Pos , a_Normal , a_BlendWeights , a_BlendIndices , a_NumBones	 );
		a_Normal = l_boonapply.m_Normal;
		a_Pos		= mul(l_boonapply.m_Pos, g_mVP);	//位置座標
	}
	else	//アニメーションするモデルじゃなかったら
	{
		a_Pos		= mul( a_Pos , g_mWVP );	//位置座標
	}

	l_Out.m_Pos = a_Pos;
	l_Out.m_Tex = a_Tex;				//テクスチャ座標

	//拡散光と環境光
	float3 L = normalize(-g_vLight);				//ライトの方向ベクトル
	float3 N = normalize(l_boonapply.m_Normal);					//法線ベクトル
	
//	float3 R = -E + 2.0f*dot(N,E)*N;				//フォン鏡面反射計算用ベクトル
//	float3 H = normalize(L+E);						//ブリンフォン鏡面反射計算用ベクトル	

	l_Out.m_Color =	  g_vAmb * g_vLightAmb 
					+ g_vDif * g_vLightDif;// * max(0,dot(N,L));//空を表現するために消している
					
//					+ pow( max(0,dot( L,R )) , 30 );		//フォン鏡面反射計算用
//					+ pow( max(0,dot( N,H )) , 30 );		//ブリンフォン鏡面反射計算用

	//カラー計算
	l_Out.m_Color = l_Out.m_Color * a_Color;

	//ピクセルシェーダへ（ラスタライズで変になるから正規化しない）
	l_Out.m_Normal   =  a_Normal.xyz;				//法線ベクトル
	l_Out.m_Eye		 =  g_vEyePos - a_Pos.xyz ;	//視点ベクトル

	return l_Out; 

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//ピクセルシェーダプログラム　
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------
//テクスチャ表示
//--------------
float4 PS_TEXTURE( VS_DATA a_In ) : COLOR		//引数のあとにセマンティクスを指定すると返却値がそれになる
{
	float4 decale = tex2D( Samp, a_In.m_Tex );
	return decale * a_In.m_Color ;	
}
//--------------
//テクスチャ表示光あり
//--------------
float4 PS( VS_DATA a_In ) : COLOR		//引数のあとにセマンティクスを指定すると返却値がそれになる
{

	float3 L = normalize(-g_vLight);
	float3 H = normalize( L + normalize( a_In.m_Eye ) );	//ブリンフォン鏡面反射計算用ベクトル	
	float3 N = normalize( a_In.m_Normal );

	float4 decale = tex2D( Samp, a_In.m_Tex );

	float4 Color =	a_In.m_Color
					+ pow( max(0,dot( N,H )) , 10 );		//ブリンフォン鏡面反射計算用

	//黒を透過色に設定
	Color.a = ((decale.r==0 && decale.g==0 && decale.b==0) ? 0 : 1 ); 

	return decale * a_In.m_Color ;	

}

//通常シェーダ
VertexShader vsArrayNormal[4] = {	compile vs_2_0 VS(1), 
													compile vs_2_0 VS(2),
													compile vs_2_0 VS(3),
													compile vs_2_0 VS(4)
												};

//---------------------------------------------------------------------------------------------------------------------
//テクニック	シェーダにつかう関数を指定できる
//---------------------------------------------------------------------------------------------------------------------
technique TShader
{

	pass P0
	{	
		//シェーダ
		VertexShader	=	(vsArrayNormal[g_NumBones]);
		PixelShader		=	compile ps_2_0 PS();
	}

	pass P1
	{	
		//シェーダ
		VertexShader	=	compile vs_2_0 VS_TEXTURE();
		PixelShader		=	compile ps_2_0 PS_TEXTURE();
	}

}