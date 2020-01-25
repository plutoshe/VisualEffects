Shader "Unlit/Indentupdate"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Coordinate("Coordinate", Vector) = (0,0,0,0)
        _Size("Size", float) = 1
        _Strength("Strength", Range(0,1)) = 1
        _DrawColor ("DrawColor", Color) = (1,0,0,0)
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
            fixed4 _DrawColor, _Coordinate;
            half _Strength, _Size;
            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 col = tex2D(_MainTex, i.uv);
                float update = pow(saturate(1 - distance(i.uv, _Coordinate.xy)), _Size);

                return saturate(col + _DrawColor * update * _Strength);
            }
            ENDCG
        }
    }
}
