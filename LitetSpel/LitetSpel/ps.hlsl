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
	Sphere spheres[5];
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

float mb(float3 p, float4 mb)
{
	float3 d = mb.xyz - p;
	//return smoothstep(0.0, mb.w, length(mb.xyz - p)) - 0.75;
	return 0.35 - (25.0 / dot(d, d));
	return 0.35 - ((mb.w * mb.w) / dot(mb.xyz - p, mb.xyz - p));
}

float testScene(float3 p)
{
	float minDist = 10000000.0;
	
	// sphere loop
	for (int i = 0; i < 5; ++i)
	{
		if (spheres[i].centerRadius.w <= 0.0) break;
		float currentDist = testSphere(p, spheres[i].centerRadius);
		if (minDist > currentDist)
		{
			minDist = currentDist;
		}
	}

	// box loop
	for (int i = 0; i < 100; ++i)
	{
		if (boxes[i].halfLengths.x <= 0.0) break;
		float currentDist = testBox(p - boxes[i].center.xyz, boxes[i].halfLengths.xyz);
		minDist = min(minDist, currentDist);
	}
	//float d1 = mb(p, spheres[0].centerRadius);
	//float d2 = mb(p, spheres[1].centerRadius);
	//float d3 = mb(p, spheres[2].centerRadius);
	//float d4 = mb(p, spheres[3].centerRadius);
	//float d5 = mb(p, spheres[4].centerRadius);
	//float mbDist = d1 + d2 + d3 + d4 + d5;
	//minDist = min(minDist, mbDist);

	return minDist;
}

float castRay(float3 ro, float3 rd)
{
	float dist = 170.0;
	float i;
	for (i = 0.0; i < 80.0; i += 1.0)
	{
		float currentDist = testScene(ro + rd * dist);
		dist += currentDist*1.00 + 0.01;
		if (dist > 270.0 || currentDist < 0.1) {
			break;
		}
		//if (currentDist < 0.1) {
		//	break;
		//}
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
	float minDist = 10000000.0;
	float3 color = float3(0.0, 0.0, 0.0);

	// sphere loop
	for (int i = 0; i < 5; ++i)
	{
		if (spheres[i].centerRadius.w <= 0.0) break;
		float currentDist = testSphere(p, spheres[i].centerRadius);
		if (minDist > currentDist)
		{
			minDist = currentDist;
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
	//float d1 = mb(p, spheres[0].centerRadius);
	//float d2 = mb(p, spheres[1].centerRadius);
	//float d3 = mb(p, spheres[2].centerRadius);
	//float d4 = mb(p, spheres[3].centerRadius);
	//float d5 = mb(p, spheres[4].centerRadius);
	//float mbDist = d1 + d2 + d3 + d4 + d5;

	//if (minDist > mbDist)
	//{
	//	minDist = mbDist;
	//	color = float3(0.0, 0.0, 1.0);
	//}

	if (minDist > 0.1) color = float3(0.0, 0.0, 0.0);
	return color;
}

float4 main(VS_OUT input) : SV_Target
{
	float2 resolution = float2(1280, 720);
	float2 uv = input.pos.xy / resolution;
	uv.y = 1 - uv.y;
	float3 ro = float3(spheres[0].centerRadius.x, spheres[0].centerRadius.y + 40.0, -200.0);
	float3 rd = normalize(float3(uv - 0.5, 1.0));
	float dist = castRay(ro, rd);
	float3 p = ro + rd * dist;
	float3 objectColor = getColor(p);
	//float3 objectColor = float3(0.2, 0.2, 0.2);
	float3 normal = calcNormal(p);
	//float3 normal = float3(0.0, 1.0, 0.0);
	float3 lightPos = float3(100.0, 400.0, -0.0);
	float3 lightVector = lightPos - p;
	float3 color = max(dot(normal, normalize(lightVector)), 0.0) * objectColor;
	color += 0.5 * objectColor;
	
	return float4(pow(color, 1.0/2.2), 1.0);
}
