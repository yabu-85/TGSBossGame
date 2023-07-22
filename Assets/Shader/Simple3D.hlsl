//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    float4x4 g_matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 g_matNormalTrans; // �@���̕ϊ��s��i��]�s��Ɗg��̋t�s��j
    float4x4 g_matWorld; // ���[���h�ϊ��s��
    float4 g_vecLightDir; // ���C�g�̕����x�N�g��
    float4 g_vecDiffuse; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 g_vecAmbient; // �A���r�G���g�J���[�i�e�̐F�j
    float4 g_vecSpeculer; // �X�y�L�����[�J���[�i�n�C���C�g�̐F�j
    float4 g_vecCameraPosition; // ���_�i�J�����̈ʒu�j
    float g_shuniness; // �n�C���C�g�̋����i�e�J����j
    bool g_isTexture; // �e�N�X�`���\���Ă��邩�ǂ���

};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i�P�x�j
    float4 eye : TEXCOORD1; //����
    
    float4 lightDir : TEXCOORD2;
    float lightLen : TEXCOORD3;
    float4 normal : TEXCOORD4;
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
    
    //���[���h���W�n�ł̌����̈ʒu���v�Z
    float4 worldg_vecLightDir = mul(g_vecLightDir, g_matWorld);
    
    //���C�g
    outData.lightDir = float4(worldg_vecLightDir.xyz, 1.0) - mul(pos.xyzw, g_matWorld);
    outData.lightLen = length(outData.lightDir);
    outData.lightDir = normalize(outData.lightDir);

	//�@���̕ό`
    normal.w = 0; //4�����ڂ͎g��Ȃ��̂�0
    normal = mul(normal, g_matNormalTrans); //�I�u�W�F�N�g���ό`����Ζ@�����ό`
    outData.normal = normal; //������s�N�Z���V�F�[�_�[��
    
    //�P�x�����s�N�Z���V�F�[�_�\�ցi�����ŋP�x�v�Z����̂͂��߂炵��
    outData.color = clamp(dot(mul(normal, g_matWorld), outData.lightDir), 0, 1);
    
    //�����x�N�g���i�n�C���C�g�̌v�Z�ɕK�v
    float4 worldPos = mul(pos, g_matWorld); //���[�J�����W�Ƀ��[���h�s��������ă��[���h���W��
    outData.eye = normalize(g_vecCameraPosition - worldPos); //���_���璸�_�ʒu�������Z�����������߂ăs�N�Z���V�F�[�_�[��

    return outData;
}

//������������������������������������������������������������������������������
//�s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambientSource = g_vecAmbient;
    float4 diffuse;
    float4 ambient;
    
    if (g_isTexture)
    {
		//�e�N�X�`���̐F
        diffuse = (g_texture.Sample(g_sampler, inData.uv) * inData.color) / inData.lightLen;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    else
    {
        //�}�e���A���̐F
        diffuse = (g_vecDiffuse * inData.color) / inData.lightLen;
        ambient = g_vecDiffuse * ambientSource;
    }

    //�Ō�ɂ�����l�͌��̌���������ʂ�ύX����l�A�����������x�ɂ��e����^���Ă���
    float scalar = saturate(dot(inData.lightDir.xyz, inData.normal.xyz) / inData.lightLen);

    //���̋����𒲐����邽�߂̌W���B�����Ⴂ�Ɣ����F�ɂȂ�
    float lightIntensity = 5.0;

    //�n�[�t�x�N�g���̌v�Z
    float4 halfVec = normalize(inData.eye + inData.lightDir);
    
    //�X�y�L�����[�̌v�Z
    float4 speculer = float4(0, 0, 0, 0); //�����l��0�ɂ���
    if (g_vecSpeculer.a != 0) //�X�y�L�����[�̏�񂪂����
    {
        speculer = pow(saturate(dot(halfVec, inData.normal)), g_shuniness) * g_vecSpeculer; // �n�C���C�g�����߂�
    }

    //�f�B�t���[�Y�ƃA���r�G���g������
    float4 finalColor = (diffuse + ambient) * lightIntensity + speculer;

    // ���̌����ɂ��e����K�p
    finalColor.rgb *= saturate(1.0 - scalar * 1.0);

    //�A���t�@�l��1.0�ɐݒ�
    finalColor.a = 1.0;

    return finalColor;
    
}