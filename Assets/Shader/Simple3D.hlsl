//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture: register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	g_matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	g_matNormalTrans;	// �@���̕ϊ��s��i��]�s��Ɗg��̋t�s��j
	float4x4	g_matWorld;			// ���[���h�ϊ��s��
	float4		g_vecLightDir;		// ���C�g�̕����x�N�g��
	float4		g_vecDiffuse;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4		g_vecAmbient;		// �A���r�G���g�J���[�i�e�̐F�j
	float4		g_vecSpeculer;		// �X�y�L�����[�J���[�i�n�C���C�g�̐F�j
	float4		g_vecCameraPosition;// ���_�i�J�����̈ʒu�j
	float		g_shuniness;		// �n�C���C�g�̋����i�e�J����j
	bool		g_isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���

};
//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������

struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i�P�x�j

    float4 lightDir : TEXCOORD1;
    float lightLen : TEXCOORD2;
    float4 normal : TEXCOORD3;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������

VS_OUT VS(float4 pos : POSITION, float2 uv : TEXCOORD, float4 normal : NORMAL)
{
    //�s�N�Z���V�F�[�_�[�ɓn���\���̃f�[�^
    VS_OUT outData;
    
    //���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
    //�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, g_matWVP);

    //�e�N�X�`���f�[�^���s�N�Z���V�F�[�_�[��
    outData.uv = uv;
    
    // ���[���h���W�n�ł̌����̈ʒu���v�Z
    float4 worldg_vecLightDir = mul(g_vecLightDir, g_matWorld);
    
    //���C�g
    outData.lightDir = float4(worldg_vecLightDir.xyz, 1.0) - mul(pos.xyzw, g_matWorld);

    outData.lightLen = length(outData.lightDir);
    outData.lightDir = normalize(outData.lightDir);

    //�@��
    normal = float4(normal.xyz, 0);
    outData.normal = mul(normal, g_matWVP);
    
    //�P�x�����s�N�Z���V�F�[�_�\��
    float4 light = mul(g_vecLightDir, g_matWorld);
    light = normalize(light);
    outData.normal = mul(normal, g_matWorld);
    outData.color = clamp(dot(mul(normal, g_matWorld), outData.lightDir), 0, 1);

    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������

float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse;
    float4 ambient;
    float4 wLight = float4(1, 1, 1, 1);

    //�Ō�ɂ����Ă���z �͌��̌���������ʂ�}���邽�߂̂��
    float scalar = dot(inData.lightDir.xyz, inData.normal.xyz) / (inData.lightLen * 0.7);
 
    if (scalar < 0.0f)
    {
        scalar = 0.0f;
    }

    //�e�N�X�`��������Ƃ�
    if (g_isTexture)
    {
        diffuse = (wLight * g_texture.Sample(g_sampler, inData.uv) * inData.color) / inData.lightLen;
        ambient = wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }

    //�e�N�X�`�����Ȃ��Ƃ�
    else
    {
        diffuse = (wLight * g_vecDiffuse * inData.color) / inData.lightLen;
        ambient = wLight * g_vecDiffuse * ambientSource;
    }
    return diffuse + ambient + scalar;

}