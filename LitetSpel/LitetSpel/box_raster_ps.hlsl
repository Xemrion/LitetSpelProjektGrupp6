struct VS_OUT
{
	float4 worldPos : POSITION;
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};

cbuffer Shadow {
	float4 shadowPoint; //xyz coords and intensity in w
};

float4 main(VS_OUT input) : SV_Target
{
	float3 objectColor = input.color.rgb;
	float3 lightPos = float3(100.0, 400.0, -200.0);
	float3 lightVector = normalize(lightPos);
	float3 normal = input.normal.xyz;
	if (abs(normal.x) < 0.57735f) normal.x = 0.0;
	if (abs(normal.y) < 0.57735f) normal.y = 0.0;
	if (abs(normal.z) < 0.57735f) normal.z = 0.0;
	normal = normalize(normal);
	float3 color = objectColor * 0.2;

	float3 shadowVector = shadowPoint.xyz - input.worldPos.xyz;
	float shadowFactor = 1.0;
	shadowVector *= float3(2.0, 1.0, 2.0);
	float3 shadowAttenuation = (shadowPoint.w * 1.0) / (length(shadowVector) * length(shadowVector) * 0.5);
	shadowFactor = 1.0 - clamp((shadowAttenuation * dot(normalize(shadowVector), lightVector)), 0.0, 1.0);
	color += max(dot(normal, lightVector), 0.0) * objectColor * shadowFactor;

	return float4(color, 1.0);
}
