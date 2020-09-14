#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec3      iResolution;          
uniform float     iTime;                
uniform vec4      iMouse;               
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

const float PI=3.14159265;
#define saturate(a) clamp(a, 0.0, 1.0)

vec3 GetReflection(vec3 rayDir)
{
	vec3 tex = texture(iChannel1, -rayDir.xy).xyz;
	return tex * tex;
}

vec2 matmin(vec2 a, vec2 b)
{
    if (a.x < b.x) return a;
    else return b;
}
vec2 matmax(vec2 a, vec2 b)
{
    if (a.x > b.x) return a;
    else return b;
}

// ---- shapes defined by distance fields ----
// See this site for a reference to more distance functions...
// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

// signed box distance field
float sdBox(vec3 p, vec3 radius)
{
  vec3 dist = abs(p) - radius;
  return min(max(dist.x, max(dist.y, dist.z)), 0.0) + length(max(dist, 0.0));
}
// -------------------------------------------
#define AA vec2(1.0, 11.0)
#define BB vec2(2.0, 11.0)
#define CC vec2(3.0, 11.0)
#define DD vec2(4.0, 11.0)
#define EE vec2(5.0, 11.0)
#define FF vec2(6.0, 11.0)
#define GG vec2(7.0, 11.0)
#define HH vec2(8.0, 11.0)
#define II vec2(9.0, 11.0)
#define JJ vec2(10.0, 11.0)
#define KK vec2(11.0, 11.0)
#define LL vec2(12.0, 11.0)
#define MM vec2(13.0, 11.0)
#define NN vec2(14.0, 11.0)
#define OO vec2(15.0, 11.0)
#define PP vec2(0.0, 10.0)
#define QQ vec2(1.0, 10.0)
#define RR vec2(2.0, 10.0)
#define SS vec2(3.0, 10.0)
#define TT vec2(4.0, 10.0)
#define UU vec2(5.0, 10.0)
#define VV vec2(6.0, 10.0)
#define WW vec2(7.0, 10.0)
#define XX vec2(8.0, 10.0)
#define YY vec2(9.0, 10.0)
#define ZZ vec2(10.0, 10.0)

#define aa vec2(1.0, 9.0)
#define bb vec2(2.0, 9.0)
#define cc vec2(3.0, 9.0)
#define dd vec2(4.0, 9.0)
#define ee vec2(5.0, 9.0)
#define ff vec2(6.0, 9.0)
#define gg vec2(7.0, 9.0)
#define hh vec2(8.0, 9.0)
#define ii vec2(9.0, 9.0)
#define jj vec2(10.0, 9.0)
#define kk vec2(11.0, 9.0)
#define ll vec2(12.0, 9.0)
#define mm vec2(13.0, 9.0)
#define nn vec2(14.0, 9.0)
#define oo vec2(15.0, 9.0)
#define pp vec2(0.0, 8.0)
#define qq vec2(1.0, 8.0)
#define rr vec2(2.0, 8.0)
#define ss vec2(3.0, 8.0)
#define tt vec2(4.0, 8.0)
#define uu vec2(5.0, 8.0)
#define vv vec2(6.0, 8.0)
#define ww vec2(7.0, 8.0)
#define xx vec2(8.0, 8.0)
#define yy vec2(9.0, 8.0)
#define zz vec2(10.0, 8.0)
vec4 SampleFontTex(vec2 uv)
{
    // Do some tricks with the UVs to spell out "TexFont" in the middle.
    vec2 fl = floor(uv + 0.5);
    if (fl.y == 2.0) 
    {
        if (fl.x == -4.0) fl = PP;
    	else if (fl.x == -3.0) fl = AA;
    	else if (fl.x == -2.0) fl = RR;
    	else if (fl.x == -1.0) fl = KK;
    	else if (fl.x == 0.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == 1.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == 2.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == 3.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == 4.0) fl = vec2(15.0, 8.0);
    }
    else if ( fl.y == 1.0)
    {
        if (fl.x == -4.0) fl = SS;
    	else if (fl.x == -3.0) fl = UU;
    	else if (fl.x == -2.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == -1.0)  fl = vec2(15.0, 8.0);
    	else if (fl.x == 0.0)  fl = vec2(15.0, 8.0);
    	else if (fl.x == 1.0)  fl = vec2(15.0, 8.0);
    	else if (fl.x == 2.0)  fl = vec2(15.0, 8.0); 
    	else if (fl.x == 3.0)  fl = vec2(15.0, 8.0); 
    	else if (fl.x == 4.0)  fl = vec2(15.0, 8.0); 
    }
    else if ( fl.y == 0.0)
    {
        if (fl.x == -4.0) fl = GG;
    	else if (fl.x == -3.0) fl = WW;
    	else if (fl.x == -2.0) fl = AA;
    	else if (fl.x == -1.0) fl = NN;
    	else if (fl.x == 0.0) fl = GG;
    	else if (fl.x == 1.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == 2.0) fl = vec2(15.0, 8.0); 
		else if (fl.x == 3.0) fl = vec2(15.0, 8.0);
    	else if (fl.x == 4.0) fl = vec2(15.0, 8.0); 
    }
    else if ( fl.y == -1.0)
    {
        if (fl.x == -4.0) fl = RR;
    	else if (fl.x == -3.0) fl = OO;
    	else if (fl.x == -2.0) fl = GG;
    	else if (fl.x == -1.0) fl = UU;
    	else if (fl.x == 0.0) fl = EE;
    	else if (fl.x == 1.0) fl = LL;
    	else if (fl.x == 2.0) fl = II;
        else if (fl.x == 3.0) fl = KK;
        else if (fl.x == 4.0) fl = EE;
    }
    else if ( fl.y == -2.0)
    {
        if (fl.x == -4.0) fl = DD;
    	else if (fl.x == -3.0) fl = UU;
    	else if (fl.x == -2.0) fl = NN;
    	else if (fl.x == -1.0) fl = GG;
    	else if (fl.x == 0.0) fl = EE;
    	else if (fl.x == 1.0) fl = OO;
    	else if (fl.x == 2.0) fl = NN;
        else if (fl.x == 3.0) fl = vec2(15.0, 8.0);
        else if (fl.x == 4.0) fl = vec2(15.0, 8.0);
    }
    uv = fl + fract(uv + 0.5) - 0.5;

    // Sample the font texture. Make sure to not use mipmaps.
    // Add a small amount to the distance field to prevent a strange bug on some gpus. Slightly mysterious. :(
    return texture(iChannel0, (uv+0.5)*(1.0/16.0), -100.0) + vec4(0.0, 0.0, 0.0, 0.000000001);
}

// This is the distance function that defines all the scene's geometry.
// The input is a position in space.
// The output is the distance to the nearest surface and a material index.
vec2 DistanceToObject(vec3 p)
{
	// Load the font texture's distance field.
    float letterDistField = (SampleFontTex(p.xy).w - 0.5+1.0/256.0);
    // intersect it with a box.
    float cropBox = sdBox(p, vec3(0.5 + 7.0, 4.5, 0.35));
    vec2 letters = matmax(vec2(letterDistField, 0.0), vec2(cropBox, 1.0));
    return letters;
}

// Input is UV coordinate of pixel to render.
// Output is RGB color.
vec3 RayTrace(in vec2 fragCoord )
{
	vec3 camPos, camUp, camLookat;
	// ------------------- Set up the camera rays for ray marching --------------------
    // Map uv to [-1.0..1.0]
	vec2 uv = fragCoord.xy/iResolution.xy * 2.0 - 1.0;
    uv /= 2.0;  // zoom in

#ifdef MANUAL_CAMERA
    // Camera up vector.
	camUp=vec3(0,1,0);

	// Camera lookat.
	camLookat=vec3(0,0.0,0);

    // debugging camera
    float mx=-iMouse.x/iResolution.x*PI*2.0;
	float my=iMouse.y/iResolution.y*3.14*0.95 + PI/2.0;
	camPos = vec3(cos(my)*cos(mx),sin(my),cos(my)*sin(mx))*7.0;
#else
    // Camera up vector.
	camUp=vec3(0,1,0);

	// Camera lookat.
	camLookat=vec3(0,0,0);

    float camSpeed = 0.3;
	float camRad = 12.0;
    float camAngle = sin(iTime*camSpeed) * 0.5;
	camPos = vec3(sin(camAngle)*camRad, 0.0, -cos(camAngle)*camRad);
#endif

	// Camera setup for ray tracing / marching
	vec3 camVec=normalize(camLookat - camPos);
	vec3 sideNorm=normalize(cross(camUp, camVec));
	vec3 upNorm=cross(camVec, sideNorm);
	vec3 worldFacing=(camPos + camVec);
	vec3 worldPix = worldFacing + uv.x * sideNorm * (iResolution.x/iResolution.y) + uv.y * upNorm;
	vec3 rayVec = normalize(worldPix - camPos);

	// ----------------------------- Ray march the scene ------------------------------
	vec2 distAndMat;  // Distance and material
	float t = 0.05;
	const float maxDepth = 18.0; // farthest distance rays will travel
	vec3 pos = camPos;
    const float smallVal = 1.0 / 16384.0;
    // ray marching time
    for (int i = 0; i < 80; i++)	// This is the count of the max times the ray actually marches.
    {
        // Step along the ray.
        pos = (camPos + rayVec * t);
        // This is _the_ function that defines the "distance field".
        // It's really what makes the scene geometry. The idea is that the
        // distance field returns the distance to the closest object, and then
        // we know we are safe to "march" along the ray by that much distance
        // without hitting anything. We repeat this until we get really close
        // and then break because we have effectively hit the object.
        distAndMat = DistanceToObject(pos);

        // move along the ray a safe amount
        t += distAndMat.x;
        // If we are very close to the object, let's call it a hit and exit this loop.
        if ((t > maxDepth) || (abs(distAndMat.x) < smallVal)) break;
    }

	// --------------------------------------------------------------------------------
	// Now that we have done our ray marching, let's put some color on this geometry.
	vec3 finalColor = vec3(0.0);

	// If a ray actually hit the object, let's light it.
    if (t <= maxDepth)
	{
        // calculate the normal from the distance field. The distance field is a volume, so if you
        // sample the current point and neighboring points, you can use the difference to get
        // the normal. This is called a gradient.
        vec3 smallVec = vec3(1.0/4096.0, 0, 0);
        vec3 normalU = vec3(DistanceToObject(pos + smallVec.xyy).x - DistanceToObject(pos - smallVec.xyy).x,
                           DistanceToObject(pos + smallVec.yxy).x - DistanceToObject(pos - smallVec.yxy).x,
                           DistanceToObject(pos + smallVec.yyx).x - DistanceToObject(pos - smallVec.yyx).x)*0.5;
        // If the material says we are on the edge of the font, override the normal with the
        // font texture's gradient. This will give us smoother surfaces on the sides of the font's letters.
        if (distAndMat.y == 0.0) {
		    vec4 tx = SampleFontTex(pos.xy) - 0.5;
            // Put a small number in Z so it can't go to zero.
            normalU = -vec3(-tx.g, tx.b, 0.0001)*2.0*smallVec.x;
        }
        // It looks like ??texture filtering?? can sometimes take the normal to zero length.
        // Compensate so we don't divide by zero in the normalize function.
        normalU = normalU+0.000000001;
        vec3 normal = normalize(normalU);

        // ------ Calculate texture color  ------
        vec3 texColor = vec3(0.0,0.0,0.0) * 0.5;
        if (distAndMat.y == 1.0) {
            // Make the "TexFont" word yellow
            if ((abs(floor(pos.y+0.5)) <= 2.0) && (abs(floor(pos.x+0.5)) <= 4.5)) texColor = vec3(1.0,0.9,0.0);
        }

        // ------ Calculate lighting color ------
        // Sky color
        vec3 lightColor = (saturate(normal.y * 0.5 + 0.5)) * vec3(2.5);

        // Apply the light to the texture.
        finalColor = texColor * lightColor;

        // calculate the reflection vector for highlights
        vec3 ref = reflect(rayVec, normal);
        vec3 refColor = GetReflection(ref);
        refColor *= vec3(1.0, 0.5, 0.2)*2.5*dot(normal, -rayVec);
        finalColor += refColor;

        // debug visualize length of gradient of distance field to check distance field correctness
        //finalColor = vec3(0.5) * (length(normalU) / smallVec.x);
        //finalColor = normal * 0.5 + 0.5;
	}
    else
    {
        // Our ray trace hit nothing, so draw background.
        finalColor = GetReflection(rayVec);
    }

	// output the final color without gamma correction - will do gamma later.
	return vec3(clamp(finalColor, 0.0, 1.0));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec3 finalColor = RayTrace(fragCoord);

    // Render 2d text.
    vec2 uv = fragCoord/iResolution.xy;
    // Render entire font texture for the first few seconds.
    float letters = texture(iChannel0, uv, -1.0).x;
    finalColor = mix(finalColor, vec3(1.0, 1.0, 1.0)*letters, saturate(0.9-iTime*0.7));

    fragColor = vec4(sqrt(clamp(finalColor, 0.0, 1.0)),1.0);
}