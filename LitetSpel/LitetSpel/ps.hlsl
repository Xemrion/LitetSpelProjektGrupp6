Texture2D background : register(t0);
Texture2D depthbuffer : register(t1);
SamplerState samp;

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

struct Sphere {
	float4 centerRadius;
};

cbuffer SphereBuffer {
	Sphere spheres[5];
};

cbuffer Camera {
	float3 cameraPos;
};

cbuffer Corners {
	float4 upLeft;
	float4 upRight;
	float4 downLeft;
	float4 downRight;
};

cbuffer Matrices {
	float4x4 viewProj;
};

float testSphere(float3 p, float4 s)
{
	return length(s.xyz - p) - s.w;
}

float mb(float3 p, float4 mb)
{
	float3 d = mb.xyz - p;
	return 0.35 - ((mb.w * mb.w) / dot(mb.xyz - p, mb.xyz - p));
}

float testScene(float3 p)
{
	float d1 = mb(p, spheres[0].centerRadius);
	float d2 = mb(p, spheres[1].centerRadius);
	float d3 = mb(p, spheres[2].centerRadius);
	float d4 = mb(p, spheres[3].centerRadius);
	float d5 = mb(p, spheres[4].centerRadius);
	float mbDist = d1 + d2 + d3 + d4 + d5;

	return mbDist;
}

float castRay(float3 ro, float3 rd, out bool intersect)
{
	float dist = 170.0;
	float i;
	intersect = false;
	for (i = 0.0; i < 80.0; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist);
		dist += currentDist * 1.00;
		if (dist > 370.0) {
			break;
		}
		else if (currentDist < 0.1) {
			intersect = true;
			break;
		}
	}

	return dist;
}

float3 calcNormal(float3 p)
{
	const float h = 0.0001;
	const float2 k = float2(1, -1);
	return normalize(k.xyy * testScene(p + k.xyy * h) +
		k.yyx * testScene(p + k.yyx * h) +
		k.yxy * testScene(p + k.yxy * h) +
		k.xxx * testScene(p + k.xxx * h));
}

float3 getColor(float3 p)
{
	float3 color = float3(0.0, 0.0, 0.0);

	float d1 = mb(p, spheres[0].centerRadius);
	float d2 = mb(p, spheres[1].centerRadius);
	float d3 = mb(p, spheres[2].centerRadius);
	float d4 = mb(p, spheres[3].centerRadius);
	float d5 = mb(p, spheres[4].centerRadius);
	float mbDist = d1 + d2 + d3 + d4 + d5;

	if (mbDist < 0.1) color = float3(1.0, 0.0, 0.0);
	return color;
}

float4 main(VS_OUT input) : SV_Target
{
	float2 resolution = float2(1280, 720);
	float2 uv = input.pos.xy / resolution;
	float3 ro = cameraPos;
	float3 rd = normalize(lerp(
		float3(lerp(upLeft.xyz, downLeft.xyz, uv.y)),
		float3(lerp(upRight.xyz, downRight.xyz, uv.y)),
		uv.x));

	bool intersect;
	float dist = castRay(ro, rd, intersect);
	float3 color = float3(0.0, 0.0, 0.0);

	if (!intersect) {
		color = background.Sample(samp, uv);
	}
	else {
		float4 projected = mul(float4(ro + rd * dist, 1.0), viewProj);
		projected.xyz /= projected.w;

		if (projected.z > depthbuffer.Sample(samp, uv).x) {
			color = background.Sample(samp, uv);
		}
		else {
			float3 p = ro + rd * dist;
			float3 objectColor = getColor(p);
			float3 normal = calcNormal(p);
			float3 lightPos = float3(100.0, 400.0, -0.0);
			float3 lightVector = lightPos - p;
			color = max(dot(normal, normalize(lightVector)), 0.0) * objectColor;
			color += 0.5 * objectColor;
		}
	}

	return float4(pow(color, 1.0 / 2.2), 1.0);
}
