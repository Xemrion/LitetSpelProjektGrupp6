struct VS_IN
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

cbuffer Transforms {
	float4x4 worldTransforms[100];
};

cbuffer ViewProj {
	float4x4 viewProj;
};

VS_OUT main(VS_IN input, uint instanceID : SV_InstanceID)
{
	VS_OUT output = (VS_OUT)0;
	float4x4 wvp = mul(worldTransforms[instanceID], viewProj);
	output.pos = mul(float4(input.pos, 1.0), wvp);
	//wvp = transpose(inverse(wvp));
	output.normal = mul(float4(input.normal, 0.0), wvp);
	return output;
}
