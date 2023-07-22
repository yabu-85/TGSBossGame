//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture: register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	g_matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matNormalTrans;	// 法線の変換行列（回転行列と拡大の逆行列）
	float4x4	g_matWorld;			// ワールド変換行列
	float4		g_vecLightDir;		// ライトの方向ベクトル
	float4		g_vecDiffuse;		// ディフューズカラー（マテリアルの色）
	float4		g_vecAmbient;		// アンビエントカラー（影の色）
	float4		g_vecSpeculer;		// スペキュラーカラー（ハイライトの色）
	float4		g_vecCameraPosition;// 視点（カメラの位置）
	float		g_shuniness;		// ハイライトの強さ（テカリ具合）
	bool		g_isTexture;		// テクスチャ貼ってあるかどうか

};
//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────

struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（輝度）

    float4 lightDir : TEXCOORD1;
    float lightLen : TEXCOORD2;
    float4 normal : TEXCOORD3;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────

VS_OUT VS(float4 pos : POSITION, float2 uv : TEXCOORD, float4 normal : NORMAL)
{
    //ピクセルシェーダーに渡す構造体データ
    VS_OUT outData;
    
    //ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
    //スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, g_matWVP);

    //テクスチャデータをピクセルシェーダーへ
    outData.uv = uv;
    
    // ワールド座標系での光源の位置を計算
    float4 worldg_vecLightDir = mul(g_vecLightDir, g_matWorld);
    
    //ライト
    outData.lightDir = float4(worldg_vecLightDir.xyz, 1.0) - mul(pos.xyzw, g_matWorld);

    outData.lightLen = length(outData.lightDir);
    outData.lightDir = normalize(outData.lightDir);

    //法線
    normal = float4(normal.xyz, 0);
    outData.normal = mul(normal, g_matWVP);
    
    //輝度情報をピクセルシェーダ―へ
    float4 light = mul(g_vecLightDir, g_matWorld);
    light = normalize(light);
    outData.normal = mul(normal, g_matWorld);
    outData.color = clamp(dot(mul(normal, g_matWorld), outData.lightDir), 0, 1);

    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────

float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse;
    float4 ambient;
    float4 wLight = float4(1, 1, 1, 1);

    //最後にかけている奴 は光の減衰させる量を抑えるためのやつ
    float scalar = dot(inData.lightDir.xyz, inData.normal.xyz) / (inData.lightLen * 0.7);
 
    if (scalar < 0.0f)
    {
        scalar = 0.0f;
    }

    //テクスチャがあるとき
    if (g_isTexture)
    {
        diffuse = (wLight * g_texture.Sample(g_sampler, inData.uv) * inData.color) / inData.lightLen;
        ambient = wLight * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }

    //テクスチャがないとき
    else
    {
        diffuse = (wLight * g_vecDiffuse * inData.color) / inData.lightLen;
        ambient = wLight * g_vecDiffuse * ambientSource;
    }
    return diffuse + ambient + scalar;

}