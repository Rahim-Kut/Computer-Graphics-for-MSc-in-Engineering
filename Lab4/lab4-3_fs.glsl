#version 400
out vec4 o_fragment_color;

uniform sampler2D i_texture;
uniform bool i_display;
uniform uint i_frame_count;

uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform vec3 i_light_position;
uniform vec3 i_light_color;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define NUM_TRIANGLES 4
#define MAX_DEPTH 10 
#define MAX_SAMPLES 1



struct Ray { vec3 origin, dir; float weight;};

//-------------------------------------------------------------------------//
// Keep a global stack of rays for recursion

Ray ray_stack[MAX_DEPTH];
int ray_stack_size = 0;

void push( Ray ray )
{
  if (ray_stack_size < MAX_DEPTH)
  {
    ray_stack[ray_stack_size] = ray;
    ray_stack_size++;
  }
  //else stack overflow -- silently ignore
}

Ray pop() // This is included for completeness, but actually isn't needed.
{
  if (ray_stack_size > 0)
  {
    ray_stack_size--;
    return ray_stack[ray_stack_size];
  }
  // else stack underflow -- silently ignore
}

//-------------------------------------------------------------------------//

struct Material{
  vec3 color_emission;
  vec3 color_diffuse;
  vec3 color_glossy;
  float roughness;
  float reflection;
  float transmission;
  float ior;  
};

struct Intersection
{
	vec3 point;
	vec3 normal;
	Material material;
};

struct Sphere {
  float radius;
  vec3 center;
  Material material;
};

struct Triangle 
{
    vec3 v0;
    vec3 v1;
    vec3 v2;
    Material material;
};

struct Plane {
  float offset;
  vec3 normal;
  Material material;
};

struct Scene {
  Sphere spheres[NUM_SPHERES];
  Triangle triangles[NUM_TRIANGLES];
  Plane ground_plane[1];
	vec3 sun_position;
  float sun_brightness;
};







Scene scene;


void init( )
{
	// Hard-coded single point light source
	scene.sun_brightness = 1;
	scene.sun_position = vec3(6e3,  1e4, 1e4);
	
	// Initialise 5 spheres and a ground plane

 
	scene.spheres[0].center = vec3(0, 0.3, 0.5) ; 
	scene.spheres[0].radius = 0.3;
	scene.spheres[0].material.color_diffuse = vec3( 0.3, 0.1, 0.1 );
	scene.spheres[0].material.color_glossy = vec3( 1 );
	scene.spheres[0].material.color_emission = vec3( 0 );
	scene.spheres[0].material.roughness = 100;
	scene.spheres[0].material.reflection = 0.5; //reflective red ball
	scene.spheres[0].material.transmission = 0;
	scene.spheres[0].material.ior = 1;
  
	scene.spheres[1].center = vec3(0.8, 0.3, 0.8);
	scene.spheres[1].radius = 0.3;
	scene.spheres[1].material.color_diffuse = 0.5 * vec3( 0.0, 1.0, 0.0 );
	scene.spheres[1].material.color_glossy = vec3( 1 );
	scene.spheres[1].material.roughness = 10000;
	scene.spheres[1].material.color_emission = vec3( 0 );
	scene.spheres[1].material.reflection = 0.1;
	scene.spheres[1].material.transmission = 0.8; // glass green ball
	scene.spheres[1].material.ior = 1.4;

	scene.spheres[2].center = vec3(0.55, 0.1, 0.2) ;
	scene.spheres[2].radius = 0.1;
	scene.spheres[2].material.color_diffuse = 0.8 * vec3( 1.0, 0.0, 0.0 );
	scene.spheres[2].material.color_glossy = vec3( 1 );
	scene.spheres[2].material.roughness = 1000;
	scene.spheres[2].material.color_emission = vec3( 1, 0, 0 ); // glowing red ball
	scene.spheres[2].material.reflection = 0.0;
	scene.spheres[2].material.transmission = 0;
	scene.spheres[2].material.ior = 1;

	scene.spheres[3].center = vec3(0.7, 0.8, -0.5) ;
	scene.spheres[3].radius = 0.8;
	scene.spheres[3].material.color_diffuse = 0.5 * vec3( 0.2, 0.2, 0.15 );
	scene.spheres[3].material.color_glossy = vec3( 1 );
	scene.spheres[3].material.roughness = 10;
	scene.spheres[3].material.color_emission = vec3( 0 );
	scene.spheres[3].material.reflection = 0.0;
	scene.spheres[3].material.transmission = 0;
	scene.spheres[3].material.ior = 1;

	scene.spheres[4].center = vec3(-0.65, 0.6, -1) ;
	scene.spheres[4].radius = 0.6;
	scene.spheres[4].material.color_diffuse = 0.5 * vec3( 0.5, 0.4, 0.25 );
	scene.spheres[4].material.color_glossy = vec3( 1 );
	scene.spheres[4].material.roughness = 5000;
	scene.spheres[4].material.color_emission = vec3( 0 );
	scene.spheres[4].material.reflection = 0.0;
	scene.spheres[4].material.transmission = 0;
	scene.spheres[4].material.ior = 1;

	scene.ground_plane[0].normal = vec3(0,1,0);
	scene.ground_plane[0].offset = 0;
	scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
	scene.ground_plane[0].material.color_glossy = vec3( 0 );
	scene.ground_plane[0].material.roughness = 0;
	scene.ground_plane[0].material.color_emission = vec3( 0 );
	scene.ground_plane[0].material.reflection = 0.0;
	scene.ground_plane[0].material.transmission = 0;
	scene.ground_plane[0].material.ior = 1;  

    // Vertices of a tetrahedron
    vec3 p0 = vec3( 0.43, 1.10, 0.40);
    vec3 p1 = vec3(-0.15, 1.10, 0.40);
    vec3 p2 = vec3( 0.15, 0.34, 0.85);
    vec3 p3 = vec3( 0.11, 1.16, 0.85);

    // Bottom face
    scene.triangles[0].v0 = p0;
    scene.triangles[0].v1 = p2;
    scene.triangles[0].v2 = p1;

    // Front face
    scene.triangles[1].v0 = p0;
    scene.triangles[1].v1 = p1;
    scene.triangles[1].v2 = p3;

    // Right face
    scene.triangles[2].v0 = p1;
    scene.triangles[2].v1 = p2;
    scene.triangles[2].v2 = p3;

    // Left face
    scene.triangles[3].v0 = p2;
    scene.triangles[3].v1 = p0;
    scene.triangles[3].v2 = p3;

    // Material for all four faces
    Material triangle_material;
    triangle_material.color_diffuse = vec3(0.45, 0.45, 0.55);
    triangle_material.color_glossy = vec3(1.0);
    triangle_material.color_emission = vec3(0.0);
    triangle_material.roughness = 100.0;
    triangle_material.reflection = 0.1;
    triangle_material.transmission = 0.0;
    triangle_material.ior = 1.0;

    for (int i = 0; i < NUM_TRIANGLES; ++i)
    {
      scene.triangles[i].material = triangle_material;
    }
}


// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction)
{
	float emission_sky = 1*scene.sun_brightness;
	float emission_sun = 10*scene.sun_brightness*scene.sun_brightness;
  vec3 sky_color = vec3(0.35, 0.65, 0.85);
  vec3 haze_color = vec3(0.8, 0.85, 0.9);
  vec3 light_color = clamp(i_light_color,0,1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);
    
  float angle = abs(dot(direction, normalize(i_light_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


// Ray-sphere intersection
float intersect(Ray ray, Sphere s) 
{
  // Return closest distance t for a ray/sphere intersection.

  // Vector from the sphere to the ray origin
  vec3 oc = ray.origin - s.center;

  // Quadratic equation: a × t² + b × t + c = 0

  // Coefficients of the quadratic equation
  float a = dot(ray.dir, ray.dir);
  float b = 2.0 * dot(oc, ray.dir);
  float c = dot(oc, oc) - s.radius * s.radius;

  float discriminant = b * b - 4.0 * a * c; 

  // Negative discriminant means that the ray misses the sphere
  if (discriminant < 0.0)
	return -1.0;
  
  float sqrt_discriminant = sqrt(discriminant);
  float t_near = (-b - sqrt_discriminant) / (2.0 * a);
  float t_far = (-b + sqrt_discriminant) / (2.0 * a);

  // Return the closest intersection in front of the camera
  if (t_near > 0.0)
	return t_near;
  if (t_far > 0.0)
	return t_far;

  return -1.0;
}

// Ray-plane intersection
float intersect(Ray ray, Plane p) 
{
  // Return closest distance t for a ray/plane intersection.

  float denominator = dot(p.normal, ray.dir); // measuers how directly rhe ray approaches the plane

  // A value close to 0 means the ray is paralell to the plane
  if (abs(denominator) < 0.000001)
	return -1.0;
  
  float t = -(dot(p.normal, ray.origin) + p.offset) / denominator;
  
  //ignore intersections behind the camera
  if (t <= 0.0)
	return -1.0;

  return t;

}

float intersect(Ray ray, Triangle triangle)
{
  // Triangle edges and surface normal.
  vec3 edge1 = triangle.v1 - triangle.v0;
  vec3 edge2 = triangle.v2 - triangle.v0;
  vec3 normal = normalize(cross(edge1, edge2));

  // Check whether the ray is parallel to the triangle plane
  float denominator = dot(normal, ray.dir);

  if (abs(denominator) < 0.000001)
    return -1.0;

  // Intersection distance to the triangle plane.
  float t = dot(triangle.v0 - ray.origin, normal) / denominator;

  // Ignore intersections behind the camera
  if (t <= 0.0)
    return -1.0;

  // The point where the ray reaches the plane
  vec3 point = ray.origin + t * ray.dir;

  // We need to check that the point lies on the inside of all three edges:

  // Edge from v0 to v1
  vec3 edge0_test = cross(triangle.v1 - triangle.v0, point - triangle.v0);

  if (dot(normal, edge0_test) < 0.0)
    return -1.0;

  // Edge from v1 to v2
  vec3 edge1_test = cross(triangle.v2 - triangle.v1, point - triangle.v1);

  if (dot(normal, edge1_test) < 0.0)
    return -1.0;

  // Edge from v2 to v0
  vec3 edge2_test = cross(triangle.v0 - triangle.v2, point - triangle.v2);

  if (dot(normal, edge2_test) < 0.0)
    return -1.0;

  return t;
}

// Check for intersection of a ray and all objects in the scene
Intersection intersect( Ray ray)
{
  Intersection I;
  float t = 1e32; // closest hit so far along this ray
  int id = -1;
    
  //Check for intersection with spheres
  for (int i = 0; i < NUM_SPHERES; ++i) {
    float d = intersect(ray, scene.spheres[i]);
    if (d>0 && d<=t) // if sphere `i` is closer than `t`, update `t` and `id`
    {
      t = d; 
      id = i;

      // Populate I with all the relevant data.  `id` is the closest
      // sphere that was hit, and `t` is the distance to it.

	  // Store the information for the closest sphere hit
	  I.point = ray.origin + t * ray.dir;
	  I.normal = normalize(I.point - scene.spheres[id].center); // normal = hit point - sphere centre
	  I.material = scene.spheres[id].material;

    }
  }

  // Check for intersection with triangles
  for (int i = 0; i < NUM_TRIANGLES; ++i)
  {
    float d = intersect(ray, scene.triangles[i]);

    if (d > 0.0 && d <= t)
    {
      t = d;
      I.point = ray.origin + t * ray.dir;

      vec3 edge1 = scene.triangles[i].v1 - scene.triangles[i].v0;
      vec3 edge2 = scene.triangles[i].v2 - scene.triangles[i].v0;

      I.normal = normalize(cross(edge1, edge2));
      I.material = scene.triangles[i].material;
    }
  }

  // Check for intersection with planes
  {
    float d = intersect(ray,scene.ground_plane[0]);
    if (d>0 && d<=t) // if the plane is closer than `t`, update `t`
    {
      t=d;

      // Store information for the ground-plane hit
	  I.point = ray.origin + t * ray.dir;
	  I.normal = normalize(scene.ground_plane[0].normal);
	  I.material = scene.ground_plane[0].material;
      
      // Adding a procedural checkerboard texture:
      I.material.color_diffuse = (mod(floor(I.point.x) + floor(I.point.z),2.0) == 0.0) ?
        scene.ground_plane[0].material.color_diffuse :
        vec3(1.0) - scene.ground_plane[0].material.color_diffuse;
    }
  }

  //If no sphere or plane hit, we hit the sky instead
  if (t>1e20){
    I.point = ray.dir*t;
    I.normal = -ray.dir;
    vec3 sky = simple_sky(ray.dir); // pick color from sky function

    // Sky is all emission, no diffuse or glossy shading:
    I.material.color_diffuse = 0 * sky; 
    I.material.color_glossy = 0.0 * vec3( 1 );
    I.material.roughness = 1;
    I.material.color_emission = 0.3 * sky;
    I.material.reflection = 0.0;
    I.material.transmission = 0;
    I.material.ior = 1;

  }
  return I;
}

vec3 raytrace() 
{
  vec3 color = vec3(0);

  {
    // Iterate over all rays on the stack
    for(int ray_stack_pos=0; ray_stack_pos < ray_stack_size; ++ray_stack_pos)
    {

      Ray ray = ray_stack[ray_stack_pos];
      Intersection isec = intersect(ray);

      vec3 nl = isec.normal * sign(-dot(isec.normal, ray.dir)); 
      vec3 light_direction = normalize(i_light_position - isec.point);

      vec3 this_color = vec3(0);

      float incoming_weight = ray.weight;      
      float reflectivity = isec.material.reflection;

      if (isec.material.transmission > 0)
      {
        // YOUR TASK: Create new ray, compute its position, its
        // direction (based on isec.material.ior) and its weight, and
        // call push(new_ray). In the next iteration of the for loop,
        // the new ray will be handled, so no recursive call to
        // raytrace() is required.

        // Check wether the ray in inside or outside the object
        bool inside = dot(isec.normal, ray.dir) > 0.0;

        // Ratio between the two indices of refraction
        float ior_ratio; 
        if (inside)
        {
            // The ray is leaving the object and entering the air
            ior_ratio = isec.material.ior;
        }
        else
        {
            // The ray is entering the object from the air
            ior_ratio = 1.0 / isec.material.ior;
        }

        // The transmitted ray
        Ray transmitted_ray;
        transmitted_ray.dir = normalize(refract(ray.dir, nl, ior_ratio));
        transmitted_ray.origin = isec.point + 0.001 * transmitted_ray.dir;
        transmitted_ray.weight = incoming_weight * isec.material.transmission; // part of the incoming weight
        
        ray.weight -= transmitted_ray.weight;

        push(transmitted_ray);
      }
      
      if (isec.material.reflection > 0)
      {
        // YOUR TASK: Create new ray, compute its position, direction,
        // and weight, and call push(ray).

        // A ray reflected around the surface normal
        Ray reflected_ray;
        reflected_ray.dir = normalize(reflect(ray.dir, nl));

        // Move the ray slightly away from the surface
        reflected_ray.origin = isec.point + 0.001 * reflected_ray.dir;

        // Split the incoming ray's weight
        reflected_ray.weight = incoming_weight * reflectivity;
        ray.weight -= reflected_ray.weight;

        push(reflected_ray);
      }

      
      // Now handle non-specular scattering (i.e., the non-recursive case)
      {

        // YOUR TASK: Create a "shadow feeler" ray and check if this
        // point is visible from the (single) light source.
        // If it is in shadow, set a black (or dark "ambient") colour.

        // Shadow ray from surface toward the light
        Ray shadow_ray;
        shadow_ray.origin = isec.point + 0.001 * light_direction; // 0.001 to move it slightly above surface and avoid shadow acne
        shadow_ray.dir = light_direction;
        shadow_ray.weight = 0.0;

        float light_distance = length(i_light_position - shadow_ray.origin);
        bool in_shadow = false;

        // Check whether a sphere blocks the light
        for (int i = 0; i < NUM_SPHERES; ++i)
        {
            float sphere_distance = intersect(shadow_ray, scene.spheres[i]);

            if (sphere_distance > 0.0 && sphere_distance < light_distance)
            {
                in_shadow = true;
                break;
            }
        }

        // Check whether a triangle blocks the light
        if (!in_shadow)
        {
            for (int i = 0; i < NUM_TRIANGLES; ++i)
            {
                float triangle_distance = intersect(shadow_ray, scene.triangles[i]);

                if (triangle_distance > 0.0 && triangle_distance < light_distance)
                {
                    in_shadow = true;
                    break;
                }
            }
        }

        // If no object blocks the light check ground plane
        if (!in_shadow)
        {
            float plane_distance = intersect(shadow_ray, scene.ground_plane[0]);

            if (plane_distance > 0.0 && plane_distance < light_distance)
            {
                in_shadow = true;
            }
        }

        // Use weak ambient illumination in shadows and full illumination otherwise
        vec3 irradiance = in_shadow ? vec3(0.05) : vec3(1.0);


        // YOUR TASK: If the point is not in shadow, compute the
        // colour here (using your BRDF, as before).

        // Lambertian shading from Task 4.1.
        float cosine = max(0.0, dot(nl, light_direction));
        vec3 lambertian_brdf =
        isec.material.color_diffuse / 3.14159265;

        this_color = ray.weight * (irradiance * lambertian_brdf * cosine + isec.material.color_emission);

        color += this_color;

      }


    }
  }

  return color;
}

void main() {

  vec2 tex_coords = gl_FragCoord.xy / i_window_size.xy;

  if(i_display)    
  {
    o_fragment_color = texture(i_texture,tex_coords);
  }    
  else
  {

    init();

    // Centre the pixel coordinates around the middle of the window.
    vec2 uv = gl_FragCoord.xy - 0.5 * i_window_size.xy;

    // Focal distance and the right-mouse zoom.
    float f_dist = i_focal_dist + i_focal_dist * i_mouse_state.w;

    // Camera basis directions.
    vec3 cx = i_right;
    vec3 cy = i_up;
    vec3 cz = i_dir;

    // Initial camera ray.
    Ray ray;
    ray.origin = i_position;
    ray.dir = normalize(f_dist * cz + uv.x * cx + uv.y * cy);
    ray.weight = 1.0;


    // Push the ray noto the ray stack
    push( ray );
    
    vec3 color = raytrace(); 
    

    // gamma corrected output color, and blended over several frames (good for path tracer)
    o_fragment_color = (texture(i_texture,tex_coords)*i_frame_count + vec4( pow ( clamp(color.xyz/MAX_SAMPLES, 0., 1.), vec3(1./2.2)), 1.))/float(1+ i_frame_count); 

  }
}
