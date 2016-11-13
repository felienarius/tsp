/* Square Matrix Class */
class Matrix
{
private:
  int dimension;
  int **matrix;
public:
  Matrix(int dim = 0)
  {
    dimension = dim;
    if(dimension <= 0)
    {
      matrix = NULL;
    }
    else
    {
      matrix = new int*[dimension];
      for (int i = 0; i < n; ++i)
      {
        matrix[i] = new int[dimension];
      }
    }
  };
  ~Matrix()
  {
    if(matrix != NULL)
    {
      for (int i = 0; i < dimension; ++i)
        delete []matrix[i];
      delete []matrix;
    }
    matrix = NULL;
  };
  
};