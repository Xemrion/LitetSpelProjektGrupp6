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

struct HitRecord {
	float3 normal;
	float3 color;
};

float testBox(float3 p, float3 b)
{
	float3 d = abs(p) - b;
	return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float testSphere(float3 p, float4 s)
{
	return length(s.xyz - p) - s.w;
}

float testScene(float3 p, out float3 normal, out float3 color)
{
	float minDist = 10000000.0;
	normal = float3(0.0, 0.0, -1.0);
	color = float3(0.0, 1.0, 0.0);
	
	// sphere loop
	for (int i = 0; i < 100; ++i)
	{
		if (spheres[i].centerRadius.w <= 0.0) break;
		float currentDist = testSphere(p, spheres[i].centerRadius);
		if (minDist > currentDist)
		{
			minDist = currentDist;
			//normal = normalize(p - spheres[i].centerRadius.xyz);
			color = float3(0.0, 0.0, 1.0);
		}
	}

	// box loop
	for (int i = 0; i < 100; ++i)
	{
		if (boxes[i].halfLengths.x <= 0.0) break;
		float currentDist = testBox(p - boxes[i].center.xyz, boxes[i].halfLengths.xyz);
		if (minDist > currentDist)
		{
			minDist = currentDist;
			color = float3(1.0, 0.0, 0.0);
		}
	}

	return minDist;
}

float castRay(float3 ro, float3 rd, out HitRecord rec)
{
	float dist = 170.0;
	float i;
	for (i = 0.0; i < 100.0; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist, rec.normal, rec.color);
		dist += currentDist;
		if (dist > 350.0) {
			rec.color = float3(0.0, 0.0, 0.0);
			rec.normal = float3(0.0, 0.0, -1.0);
			break;
		}
		if (currentDist < 0.1) {
			break;
		}
	}

	return dist;
}

float3 calcNormal(float3 p)
{
	const float h = 0.0001;
	const float2 k = float2(1, -1);
	float3 dn;
	float3 dc;
	return normalize(k.xyy * testScene(p + k.xyy * h, dn, dc) +
					 k.yyx * testScene(p + k.yyx * h, dn, dc) +
					 k.yxy * testScene(p + k.yxy * h, dn, dc) +
					 k.xxx * testScene(p + k.xxx * h, dn, dc));
}

float4 main(VS_OUT input) : SV_Target
{
	float2 resolution = float2(1280, 720);
	float2 uv = input.pos.xy / resolution;
	uv.y = 1 - uv.y;
	float3 ro = float3(spheres[0].centerRadius.x, spheres[0].centerRadius.y + 40.0, -200.0);
	float3 rd = normalize(float3(uv - 0.5, 1.0));
	HitRecord rec;
	float dist = castRay(ro, rd, rec);
	float3 p = ro + rd * dist;
	rec.normal = calcNormal(p);
	float3 lightPos = float3(100.0, 400.0, -400.0);
	float3 lightVector = lightPos - p;
	float3 color = max(dot(rec.normal, normalize(lightVector)), 0.0) * rec.color;
	color += 0.5 * rec.color;
	
	return float4(pow(color, 1.0/2.2), 1.0);
}
