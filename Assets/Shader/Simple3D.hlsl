//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 g_matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 g_matNormalTrans; // 法線の変換行列（回転行列と拡大の逆行列）
    float4x4 g_matWorld; // ワールド変換行列
    float4 g_vecLightDir; // ライトの方向ベクトル
    float4 g_vecDiffuse; // ディフューズカラー（マテリアルの色）
    float4 g_vecAmbient; // アンビエントカラー（影の色）
    float4 g_vecSpeculer; // スペキュラーカラー（ハイライトの色）
    float4 g_vecCameraPosition; // 視点（カメラの位置）
    float g_shuniness; // ハイライトの強さ（テカリ具合）
    bool g_isTexture; // テクスチャ貼ってあるかどうか

};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（輝度）
    float4 eye : TEXCOORD1; //視線
    
    float4 lightDir : TEXCOORD2;
    float lightLen : TEXCOORD3;
    float4 normal : TEXCOORD4;
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
    
    //ワールド座標系での光源の位置を計算
    float4 worldg_vecLightDir = mul(g_vecLightDir, g_matWorld);
    
    //ライト
    outData.lightDir = float4(worldg_vecLightDir.xyz, 1.0) - mul(pos.xyzw, g_matWorld);
    outData.lightLen = length(outData.lightDir);
    outData.lightDir = normalize(outData.lightDir);

	//法線の変形
    normal.w = 0; //4次元目は使わないので0
    normal = mul(normal, g_matNormalTrans); //オブジェクトが変形すれば法線も変形
    outData.normal = normal; //これをピクセルシェーダーへ
    
    //輝度情報をピクセルシェーダ―へ（ここで輝度計算するのはだめらしい
    outData.color = clamp(dot(mul(normal, g_matWorld), outData.lightDir), 0, 1);
    
    //視線ベクトル（ハイライトの計算に必要
    float4 worldPos = mul(pos, g_matWorld); //ローカル座標にワールド行列をかけてワールド座標へ
    outData.eye = normalize(g_vecCameraPosition - worldPos); //視点から頂点位置を引き算し視線を求めてピクセルシェーダーへ

    return outData;
}

//───────────────────────────────────────
//ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambientSource = g_vecAmbient;
    float4 diffuse;
    float4 ambient;
    
    if (g_isTexture)
    {
		//テクスチャの色
        diffuse = (g_texture.Sample(g_sampler, inData.uv) * inData.color) / inData.lightLen;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    else
    {
        //マテリアルの色
        diffuse = (g_vecDiffuse * inData.color) / inData.lightLen;
        ambient = g_vecDiffuse * ambientSource;
    }

    //最後にかける値は光の減衰させる量を変更する値、ただし透明度にも影響を与えている
    float scalar = saturate(dot(inData.lightDir.xyz, inData.normal.xyz) / inData.lightLen);

    //光の強さを調整するための係数。ここ低いと薄い色になる
    float lightIntensity = 5.0;

    //ハーフベクトルの計算
    float4 halfVec = normalize(inData.eye + inData.lightDir);
    
    //スペキュラーの計算
    float4 speculer = float4(0, 0, 0, 0); //初期値を0にする
    if (g_vecSpeculer.a != 0) //スペキュラーの情報があれば
    {
        speculer = pow(saturate(dot(halfVec, inData.normal)), g_shuniness) * g_vecSpeculer; // ハイライトを求める
    }

    //ディフューズとアンビエントを合成
    float4 finalColor = (diffuse + ambient) * lightIntensity + speculer;

    // 光の減衰による影響を適用
    finalColor.rgb *= saturate(1.0 - scalar * 1.0);

    //アルファ値を1.0に設定
    finalColor.a = 1.0;

    return finalColor;
    
}