//---------------------------------------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//---------------------------------------------------------------------------------------------------------------------

//�r���[�Ǝˉe��Ԃւ̍��W�ϊ�
float4x4 g_mVP;
//���[�J�����W����ˉe��Ԃւ̍��W�ϊ�
float4x4 g_mWVP;
		
//�{�[���p�ϊ��s��̔z��
static const int MAX_MATRICES = 26;
float4x3 g_mWorldMatrixArray[MAX_MATRICES];
//�{�[����(0�̂Ƃ��͒ʏ탌���_�����O)
int g_NumBones=2;

//�����̐ݒ�(�@���l���̉A�e�v�Z�p�����x�N�g��)
float3 g_vLight;

//�J�����̈ʒu(�ʏ�`��p(���z�Ƃ���))
float3 g_vEyePos;

//���̋���(���[�U����)
float4 g_vLightAmb = { 1.0f, 1.0f, 1.0f, 1.0f };
float4 g_vLightDif = { 1.0f, 1.0f, 1.0f, 1.0f };

//���ۂ̃��C�e�B���O���̋���
float4 g_vAmb = { 0.1f, 0.1f, 0.1f, 1.0f };
float4 g_vDif = { 0.8f, 0.8f, 0.8f, 1.0f };

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�e�N�X�`��
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

//XFILE�̃e�N�X�`��
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
//���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���\����
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct VS_DATA
{
	float4 m_Pos			:POSITION0;		//���_�f�[�^
	float2 m_Tex			:TEXCOORD0_centroid;		//�e�N�X�`���f�[�^
	
	float4 m_Color			:COLOR0;		//�J���[	

	float3 m_Normal			:TEXCOORD1;		//�@���x�N�g��
	float3 m_Eye				:TEXCOORD2;		//���_�x�N�g��

};

struct VS_DATA_SHADOW
{
	float4 m_Pos			:POSITION0;		//���_�f�[�^
	float2 m_Tex			:TEXCOORD0_centroid;		//�e�N�X�`���f�[�^
	float4 m_WorldPos		:TEXCOORD7;		//���[���h�ϊ���̒��_�f�[�^
	
	float4 m_Ambient		:COLOR0;		//����			
	float4 m_Diffuse		:COLOR1;		//�g�U��

	float3 m_Normal			:TEXCOORD1;		//�@���x�N�g��
	float3 m_Eye			:TEXCOORD2;		//���_�x�N�g��
	
	float4 m_ShadowMapUV	:TEXCOORD3;		//�V���h�EUV���W
	float4 m_Depth			:TEXCOORD4;		//�[�x��r�p	
	float4 m_Shadow			:TEXCOORD5;		//�e��r�p

	float3 m_LightEye		:TEXCOORD6;		//���C�g�̎��_�̈ʒu�x�N�g��(���W�s�N�Z������)

};

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�⏕�v���O�����@
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------
//�{�[����K��
//--------------
VS_DATA ApplyBoon(
	float4 a_Pos					:POSITION,			//���_�f�[�^	
	float3 a_Normal				:NORMAL,				//�@���f�[�^
	float4  a_BlendWeights   : BLENDWEIGHT,	//�u�����f�B���O�̏d��
    float4  a_BlendIndices    : BLENDINDICES,	//�u�����h�̃C���f�b�N�X
	uniform int NumBones								//�{�[����
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
//���_�V�F�[�_�v���O�����@
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------
//�e�N�X�`���\��
//--------------
VS_DATA VS_TEXTURE(
	float4 a_Pos					:POSITION,			//���_�f�[�^	
	float2 a_Tex					:TEXCOORD,			//�e�N�X�`���f�[�^
	float4 a_Color					:COLOR		//���_�f�[�^	
){

	VS_DATA l_Out	= (VS_DATA)0;			//�o�̓f�[�^

	l_Out.m_Pos = a_Pos = mul( a_Pos , g_mWVP );	//�ʒu���W ;

	l_Out.m_Tex = a_Tex;				//�e�N�X�`�����W

	l_Out.m_Color =	  g_vAmb + g_vDif ;

	//�J���[�v�Z
	l_Out.m_Color = l_Out.m_Color * a_Color;

	return l_Out; 

}
//--------------
//�e�N�X�`���\��������
//--------------
VS_DATA VS(
	float4 a_Pos					:POSITION,			//���_�f�[�^	
	float2 a_Tex					:TEXCOORD,			//�e�N�X�`���f�[�^
	float3 a_Normal				:NORMAL,				//�@���f�[�^
	float4 a_Color					:COLOR,			//���_�f�[�^	
	float4  a_BlendWeights   : BLENDWEIGHT,	//�u�����f�B���O�̏d��
    float4  a_BlendIndices    : BLENDINDICES,	//�u�����h�̃C���f�b�N�X
	uniform int a_NumBones								//�{�[����
){

	VS_DATA l_Out	= (VS_DATA)0;			//�o�̓f�[�^
	VS_DATA l_boonapply = (VS_DATA)0;

	//�{�[����񂪂�������
	if(g_NumBones != 0)
	{
		//�{�[���̓K�p
		l_boonapply = ApplyBoon( a_Pos , a_Normal , a_BlendWeights , a_BlendIndices , a_NumBones	 );
		a_Normal = l_boonapply.m_Normal;
		a_Pos		= mul(l_boonapply.m_Pos, g_mVP);	//�ʒu���W
	}
	else	//�A�j���[�V�������郂�f������Ȃ�������
	{
		a_Pos		= mul( a_Pos , g_mWVP );	//�ʒu���W
	}

	l_Out.m_Pos = a_Pos;
	l_Out.m_Tex = a_Tex;				//�e�N�X�`�����W

	//�g�U���Ɗ���
	float3 L = normalize(-g_vLight);				//���C�g�̕����x�N�g��
	float3 N = normalize(l_boonapply.m_Normal);					//�@���x�N�g��
	
//	float3 R = -E + 2.0f*dot(N,E)*N;				//�t�H�����ʔ��ˌv�Z�p�x�N�g��
//	float3 H = normalize(L+E);						//�u�����t�H�����ʔ��ˌv�Z�p�x�N�g��	

	l_Out.m_Color =	  g_vAmb * g_vLightAmb 
					+ g_vDif * g_vLightDif;// * max(0,dot(N,L));//���\�����邽�߂ɏ����Ă���
					
//					+ pow( max(0,dot( L,R )) , 30 );		//�t�H�����ʔ��ˌv�Z�p
//					+ pow( max(0,dot( N,H )) , 30 );		//�u�����t�H�����ʔ��ˌv�Z�p

	//�J���[�v�Z
	l_Out.m_Color = l_Out.m_Color * a_Color;

	//�s�N�Z���V�F�[�_�ցi���X�^���C�Y�ŕςɂȂ邩�琳�K�����Ȃ��j
	l_Out.m_Normal   =  a_Normal.xyz;				//�@���x�N�g��
	l_Out.m_Eye		 =  g_vEyePos - a_Pos.xyz ;	//���_�x�N�g��

	return l_Out; 

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//�s�N�Z���V�F�[�_�v���O�����@
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------
//�e�N�X�`���\��
//--------------
float4 PS_TEXTURE( VS_DATA a_In ) : COLOR		//�����̂��ƂɃZ�}���e�B�N�X���w�肷��ƕԋp�l������ɂȂ�
{
	float4 decale = tex2D( Samp, a_In.m_Tex );
	return decale * a_In.m_Color ;	
}
//--------------
//�e�N�X�`���\��������
//--------------
float4 PS( VS_DATA a_In ) : COLOR		//�����̂��ƂɃZ�}���e�B�N�X���w�肷��ƕԋp�l������ɂȂ�
{

	float3 L = normalize(-g_vLight);
	float3 H = normalize( L + normalize( a_In.m_Eye ) );	//�u�����t�H�����ʔ��ˌv�Z�p�x�N�g��	
	float3 N = normalize( a_In.m_Normal );

	float4 decale = tex2D( Samp, a_In.m_Tex );

	float4 Color =	a_In.m_Color
					+ pow( max(0,dot( N,H )) , 10 );		//�u�����t�H�����ʔ��ˌv�Z�p

	//���𓧉ߐF�ɐݒ�
	Color.a = ((decale.r==0 && decale.g==0 && decale.b==0) ? 0 : 1 ); 

	return decale * a_In.m_Color ;	

}

//�ʏ�V�F�[�_
VertexShader vsArrayNormal[4] = {	compile vs_2_0 VS(1), 
													compile vs_2_0 VS(2),
													compile vs_2_0 VS(3),
													compile vs_2_0 VS(4)
												};

//---------------------------------------------------------------------------------------------------------------------
//�e�N�j�b�N	�V�F�[�_�ɂ����֐����w��ł���
//---------------------------------------------------------------------------------------------------------------------
technique TShader
{

	pass P0
	{	
		//�V�F�[�_
		VertexShader	=	(vsArrayNormal[g_NumBones]);
		PixelShader		=	compile ps_2_0 PS();
	}

	pass P1
	{	
		//�V�F�[�_
		VertexShader	=	compile vs_2_0 VS_TEXTURE();
		PixelShader		=	compile ps_2_0 PS_TEXTURE();
	}

}