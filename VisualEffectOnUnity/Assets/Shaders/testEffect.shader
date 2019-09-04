Shader "test/testEffect"
{

	//show values to edit in inspector
	Properties{
		[HideInInspector] _MainTex("Texture", 2D) = "white" {}
		[Header(Wave)]
		_WaveDistance("Distance from player", float) = 10
		_WaveTrail("Length of the trail", Range(0,5)) = 1
		_WaveColor("Color", Color) = (1,0,0,1)
		//_InverseView("InverseView", float4x4) = (1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1)
	}

	SubShader{
		// markers that specify that we don't need culling 
		// or comparing/writing to the depth buffer
		Cull Off
		ZWrite Off
		ZTest Always

		Pass{
			CGPROGRAM
			//include useful shader functions
			#include "UnityCG.cginc"

			//define vertex and fragment shader
			#pragma vertex vert
			#pragma fragment frag

			//the rendered screen so far
			sampler2D _MainTex;

			//the depth texture
			sampler2D _CameraDepthTexture;

			//variables to control the wave
			float _WaveDistance;
			float _WaveTrail;
			float4 _WaveColor;
			float4x4 _InverseView;
			float4 _CameraPosition;


			//the object data that's put into the vertex shader
			struct appdata {
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			//the data that's used to generate fragments and can be read by the fragment shader
			struct v2f {
				float4 position : SV_POSITION;
				float2 uv : TEXCOORD0;
			};

			//the vertex shader
			v2f vert(appdata v) {
				v2f o;
				//convert the vertex positions from object space to clip space so they can be rendered
				o.position = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}

			float distance(float3 pt1, float3 pt2)
			{
				float3 v = pt2 - pt1;
				return sqrt(dot(v, v));
			}

			//the fragment shader
			fixed4 frag(v2f i) : SV_TARGET{
				float depth = LinearEyeDepth(SAMPLE_DEPTH_TEXTURE(_CameraDepthTexture, i.uv));

				float2 p11_22 = float2 (unity_CameraProjection._11, unity_CameraProjection._22);

				float3 vpos = float3((i.uv * 2 - 1) / p11_22, -1) * depth;
				float4 wpos = mul(_InverseView, float4(vpos, 1));
				fixed4 source = tex2D(_MainTex, i.uv);
			
				//float3 np = wpos - _CameraPosition;
				/*if (dot(wpos, wpos) < 100)
				{
					return  fixed4(dot(wpos, wpos) / 100.0, 0.0, 0.0, 1.0);
				}*/
				//lerp(tex2D(_MainTex, i.uv), _FogColor, saturate(exp(-worldPos.y - _Start)* _Density));
				float height = 10;
				float4 originalColor = tex2D(_MainTex, i.uv);
				float4 fogColor = float4(0.8f, 0.9f, 1.0f, 1);

				float di = sqrt(dot(vpos, vpos));
				
				
				if (di < 15)
				{
					//return fixed4(di / 15, 1.0, 1.0, 1.0);
					float fogAmount = min(1, di / 15);
					return originalColor + (fogColor - originalColor) * fogAmount;
				}
				//wpos.y = wpos.y - _CameraPosition.y;
				if (wpos.y < height)
				{	
					//return lerp(tex2D(_MainTex, i.uv), float4(0.8f,0.9f,1.0f,1), saturate(exp(-wpos.y - _CameraPosition.y)* 0.1f));
					//return fixed4(1.0, 1.0, 0.0, 1.0);
					
					float fogAmount = min(1, (height - wpos.y) / 10); //exp(abs(wpos.y - _CameraPosition.y)) * 0.1f;
					return originalColor + (fogColor - originalColor) * fogAmount;
					//return fixed4(1.0, 1.0, 0.0, 1.0);
				}
				return source;
				////get depth from depth texture
				//float depth = tex2D(_CameraDepthTexture, i.uv).r;
				////linear depth between camera and far clipping plane
				//depth = Linear01Depth(depth);
				////depth as distance from camera in units 
				//depth = depth * _ProjectionParams.z;

				////get source color
				//fixed4 source = tex2D(_MainTex, i.uv);
				////skip wave and return source color if we're at the skybox
				///*if (depth >= _ProjectionParams.z)
				//	return source;*/
				////return fixed4(depth, 0.0, 0.0, 1.0);
				//if (depth >= 10)
				//	return source;
				//else
				//	return fixed4(1.0, 1.0, 0.0, 1.0);
				////calculate wave
				//float waveFront = step(depth, _WaveDistance);
				//float waveTrail = smoothstep(_WaveDistance - _WaveTrail, _WaveDistance, depth);
				//float wave = waveFront * waveTrail;

				////mix wave into source color
				//fixed4 col = lerp(source, _WaveColor, wave);

				//return col;

			}
			ENDCG
		}
	}
}
