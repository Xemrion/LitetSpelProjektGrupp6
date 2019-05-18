struct VS_IN
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 worldPos : POSITION;
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};

cbuffer Transforms {
	float4x4 worldTransforms[50];
	float4 color[50];
};

cbuffer ViewProjMatrix {
	float4x4 viewProj;
};

VS_OUT main(VS_IN input, uint instanceID : SV_InstanceID)
{
	VS_OUT output = (VS_OUT)0;
	output.worldPos = mul(float4(input.pos, 1.0), worldTransforms[instanceID]);
	output.pos = mul(output.worldPos, viewProj);
	output.normal = float4(input.normal, 0.0);
	output.color = color[instanceID];
	return output;
}
