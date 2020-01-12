Shader "Custom/GrassVertexAni"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Noise("Noise", 2D) = "black" {}
        _WindControl("WindControl(x:XSpeed y:YSpeed z:ZSpeed w:windMagnitude)",vector) = (1,0,1,0.5)
        //前面几个分量表示在各个轴向上自身摆动的速度, w表示摆动的强度
        _WaveControl("WaveControl(x:XSpeed y:YSpeed z:ZSpeed w:worldSize)",vector) = (1,0,1,1)
        //前面几个分量表示在各个轴向上风浪的速度, w用来模拟地图的大小,值越小草摆动的越凌乱，越大摆动的越整体
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
            #pragma multi_compile_instancing

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
                UNITY_VERTEX_INPUT_INSTANCE_ID
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 pos : SV_POSITION;
                float3 tempCol : TEXCOORD1;//用来测试传递noise贴图采样的结果
            };

            sampler2D _MainTex;
            sampler2D _Noise;
            half4 _WindControl;
            half4 _WaveControl;

            v2f vert (appdata v)
            {
                // float4 minAxis = mul(unity_ObjectToWorld, float4(0,-0.5,-1,0));
                // v2f o;
                // float4 worldPos = mul(unity_ObjectToWorld, v.vertex);
                // worldPos.x += sin(_Time.x * 10) * (v.vertex.y + 0.5);
                // o.pos = mul(UNITY_MATRIX_VP, worldPos);

            //     v2f o;
            //     float4 worldPos = mul(unity_ObjectToWorld, v.vertex);
            //     float2 samplePos = worldPos.xz / _WaveControl.w;
            //     samplePos += _Time.x * -_WaveControl.xz;
            //     fixed waveSample = tex2Dlod(_Noise, float4(samplePos, 0, 0)).r;
            //     worldPos.x += sin(waveSample * _WindControl.x) * _WaveControl.x * _WindControl.w * v.uv.y;
            //     worldPos.z += sin(waveSample * _WindControl.z) * _WaveControl.z * _WindControl.w * v.uv.y;
            //     o.pos = mul(UNITY_MATRIX_VP, worldPos);
            //     o.uv = v.uv;
            //    // o.tempCol = waveSample;

            //     return o;



                v2f o;
                UNITY_SETUP_INSTANCE_ID(v);
                float4 worldPos = mul(unity_ObjectToWorld, v.vertex);
                float2 samplePos = worldPos.xz / _WaveControl.w;
                samplePos += _Time.x *-_WaveControl.xz;
                //fixed waveSample = sin(_Time.x * 10) ;//tex2Dlod(_Noise, float4(samplePos, 0, 0)).r;
                fixed waveSample =  tex2Dlod(_Noise, float4(samplePos, 0, 0)).r;
              //  fixed waveSample = tex2Dlod(_Noise, float4(samplePos, 0, 0)).r;
                worldPos.x += sin(waveSample * _WindControl.x) * _WaveControl.x * _WindControl.w * (v.vertex.y + 0.5) * 100;
                worldPos.z += sin(waveSample * _WindControl.z) * _WaveControl.z * _WindControl.w * (v.vertex.y + 0.5) * 10;
                o.pos = mul(UNITY_MATRIX_VP, worldPos);
                o.uv = v.uv;
                o.tempCol = waveSample;

                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 col = tex2D(_MainTex, i.uv);
                //return fixed4(frac(i.tempCol.x), 0, 0, 1);
                return col;
            }
            ENDCG
        }
    }
}