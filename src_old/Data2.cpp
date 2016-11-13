class Data2
{
private:
  int nodes;
  Matrix **distances_matrix;
  Matrix **travell_times;
  int **time_windows;
  int *f;
public:
  Data2(){
    nodes = 0;
    distances_matrix = travell_times = time_windows = NULL;
    f = NULL;
  };
  ~Data2(){
    if(distances_matrix != NULL)
  };
	
};