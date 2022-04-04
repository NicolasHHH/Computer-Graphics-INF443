#include "interpolation.hpp"


using namespace cgp;

vec3 interpolation(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times)
{
    // Find idx such that key_times[idx] < t < key_times[idx+1]
    int idx = find_index_of_interval(t, key_times);

    // Get parameters (time and position) used to compute the linear interpolation
    //   (You will need to get more parameters for the spline interpolation)
    float t1 = key_times[idx-1]; // = t_{i-1}
    float t2 = key_times[idx]; // = t_{i}
    float t3 = key_times[idx+1]; // = t_{i+1}
    float t4 = key_times[idx+2]; // = t_{i+2}

    vec3 const& p1 = key_positions[idx-1]; // = p_{i-1}
    vec3 const& p2 = key_positions[idx]; // = p_{i}
    vec3 const& p3 = key_positions[idx+1]; // = p_{i+1}
    vec3 const& p4 = key_positions[idx+2]; // = p_{i+2}
	
    // Call the interpolation
	// vec3 p = linear_interpolation(t, t1,t2, p1,p2);

    // tension :
    //K = 0: Courbe la plus tendue: chaque morceau est un segment de droite
    //K=0.5: Tension dite naturelle, également appelée spline de Catmull-Rom
    //K>0.5: Tangente de plus en plus marquée aux points de controles.
    // La trajectoire s'éloigne du polygone de controle,
    // et des points d'inflexions de la courbe apparaissent entre les points de controles.
    float K = 0.4;
    vec3 p = cardinal_spline_interpolation(t,t1,t2,t3,t4,p1,p2,p3,p4,K);

    return p;
}

/** Compute linear interpolation
      t: current time
      k: "current index" such that t_k < t < t_{k+1}, wxith t_k=key_time[k]
      t1: t_k 
      t2: t_{k+1} 
      (p1,p2): positions associated to time (t1,t2) */
vec3 linear_interpolation(float t, float t1, float t2, vec3 const& p1, vec3 const& p2)
{
    float const alpha = (t-t1)/(t2-t1);
    vec3 const p = (1-alpha)*p1 + alpha*p2;

    return p;
}

/** Compute the cardinal spline interpolation
      t: current time
      k: "current index" such that t_k < t < t_{k+1}, wxith t_k=key_time[k]
      t0: t_{k-1}
      t1: t_k
      t2: t_{k+1} 
      t3: t_{k+2}  
      (p0,p1,p2,p3): positions associated to time (t0,t1,t2,t3) 
      K: spline tension       */
vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, vec3 const& p0, vec3 const& p1, vec3 const& p2, vec3 const& p3, float K)
{
    // To do: fill the function to compute p(t) as a cardinal spline interpolation
    vec3 p = {0,0,0};
    float s = (t-t1)/(t2-t1);
    float di = 0;
    float di_ = 0;
    for(int i = 0;i<3;i++){
        di = 2*K*(p2[i]-p0[i])/(t2-t0);
        di_ = 2*K*(p3[i]-p1[i])/(t3-t1);
        p[i] = (2*s*s*s-3*s*s+1)*p1[i] + (s*s*s-2*s*s+s)*di + (-2*s*s*s+3*s*s)*p2[i] +(s*s*s-s*s)*di_;
    }
    return p;
}

int find_index_of_interval(float t, buffer<float> const& intervals)
{
    int const N = intervals.size();
    bool error = false;

    if (intervals.size() < 2) {
        std::cout<<"Error: Intervals should have at least two values; current size="<<intervals.size()<<std::endl;
        error = true;
    }
    if (N>0 && t < intervals[0]) {
        std::cout<<"Error: current time t is smaller than the first time of the interval"<<std::endl;
        error = true;
    }
    if(N>0 && t > intervals[N-1]) {
        std::cout<<"Error: current time t is greater than the last time of the interval"<<std::endl;
        error = true;
    }
    if (error == true) {
        std::string const error_str = "Error trying to find interval for t="+str(t)+" within values: ["+str(intervals)+"]";
        error_cgp( error_str );
    }


    int k=0;
    while( intervals[k+1]<t )
        ++k;
    return k;
}