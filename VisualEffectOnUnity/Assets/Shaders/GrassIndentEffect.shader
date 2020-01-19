Shader "Custom/GrassIndentEffect"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
        _Noise("Noise", 2D) = "black" {}
        _WindControl("WindControl(x:XSpeed y:YSpeed z:ZSpeed w:windMagnitude)",vector) = (1,0,1,0.5)
        _WaveControl("WaveControl(x:XSpeed y:YSpeed z:ZSpeed w:worldSize)",vector) = (1,0,1,1)
        _InfluencePosition("InfluencePosition",vector) = (0,0,0,1)
        _InfluenceRadius("InfluenceRadius", float) = 1
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
                float3 tempCol : TEXCOORD1;
            };

            sampler2D _MainTex;
            sampler2D _Noise;
            half4 _WindControl;
            half4 _WaveControl;
            float4 _InfluencePosition;
            half _InfluenceRadius;
            v2f vert (appdata v)
            {
               


                v2f o;
                UNITY_SETUP_INSTANCE_ID(v);
                float4 worldPos = mul(unity_ObjectToWorld, v.vertex);
                float2 samplePos = worldPos.xz / _WaveControl.w;
                samplePos += _Time.x *-_WaveControl.xz;
                fixed waveSample =  tex2Dlod(_Noise, float4(samplePos, 0, 0)).r;
             
                worldPos.x += sin(waveSample * _WindControl.x) * _WaveControl.x * _WindControl.w * (v.vertex.y + 0.5) * 100;
                worldPos.z += sin(waveSample * _WindControl.z) * _WaveControl.z * _WindControl.w * (v.vertex.y + 0.5) * 10;
                float4 top = mul(unity_ObjectToWorld, float4(v.vertex.x, 0.5, v.vertex.z, 1));
                float4 down = mul(unity_ObjectToWorld, float4(v.vertex.x, -0.5, v.vertex.z, 1));

                float3 direction = worldPos.xyz - _InfluencePosition.xyz;
                float disXZ = distance(_InfluencePosition.xz, worldPos.xz);
                float dis = distance(_InfluencePosition.xyz, worldPos.xyz);
                float pushDown = (3 + _InfluenceRadius - dis) * (v.vertex.y + 0.5);
                if (direction.y > 0)
                {
                    /*pushDown = (3 + _InfluenceRadius - disXZ) * (v.vertex.y + 0.5);
                    if (pushDown > _InfluenceRadius)
                    {
                        pushDown = _InfluenceRadius;
                    }
                    
                    if (pushDown > 0.f)
                    {
                        direction.y = 0;
                        direction = normalize(direction);
                        worldPos.xyz += direction * pushDown;
                    }*/
                }
                else
                {
                    if (pushDown > _InfluenceRadius)
                    {
                        pushDown = _InfluenceRadius;
                    }
                    if (pushDown > 0.f)
                    {
                        direction = normalize(direction);
                        worldPos.xyz += direction * pushDown;
                    }
                }

                
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