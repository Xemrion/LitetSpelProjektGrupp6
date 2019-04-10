struct VS_OUT
{
	float4 pos : SV_POSITION;
};

struct Box {
	float4 center;
	float4 halfLengths;
};

cbuffer BoxBuffer {
	Box boxes[100];
};

struct Sphere {
	float4 centerRadius;
};

cbuffer SphereBuffer {
	Sphere spheres[100];
};

float sdBox(float3 p, float3 b)
{
	float3 d = abs(p) - b;
	return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float testScene(float3 p)
{
	float minDist = 10000000.0;

	minDist = min(length(spheres[0].centerRadius.xyz - p) - spheres[0].centerRadius.w, sdBox(p - boxes[0].center.xyz, boxes[0].halfLengths.xyz));

	return minDist;
}

float3 castRay(float3 ro, float3 rd)
{
	float dist = 0.0;
	float3 color = float3(1.0, 1.0, 1.0);

	for (float i = 0.0; i < 100.0; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist);
		dist += currentDist;
		if (dist > 1000.0) {
			return float3(0.0, 1.0, 0.0);
		}
		if (currentDist < 0.1) {
			return float3(0.0, 0.0, 1.0);
		}
	}

	return color;
}

float4 main(VS_OUT input) : SV_Target
{
	float2 resolution = float2(1280, 720);
	float2 uv = input.pos.xy / resolution;
	float3 ro = float3(-4.0, -4.0, -20.0);
	float3 rd = normalize(float3(uv, 1.0));
	//float4 color = float4(uv, 0.0, 1.0);
	float3 color = castRay(ro, rd);

	return float4(color, 1.0);
}
