class Matrix
{
    private:
      int row_count;
      int column_count;
      int* data;
      
      bool destroy_data;
    public:
      Matrix(int row_count, int column_count)
      {
	  this->row_count = row_count;
	  this->column_count = column_count;
	
	  data = new int[row_count * column_count];
	  
	  destroy_data = true;
      }
      
      Matrix(int row_count, int column_count, int* data, bool destroy_data)
      {
	  this->row_count = row_count;
	  this->column_count = column_count;

	  this->data = data;
	  this->destroy_data = destroy_data;
      }
      
      void set(int row, int column, int value)
      {
	  if(row >= row_count || column >= column_count)
	      throw "Matrix::set(): Out of bounds !";
	
	  data[row * column_count + column] = value;
      }
      
      int get(int row, int column)
      {
	  if(row >= row_count || column >= column_count)
	      throw "Matrix::set(): Out of bounds !";

	  return data[row * column_count + column];
      }    
      
      int getRowCount()
      {
	  return row_count;
      }
      
      int getColumnCount()
      {
	  return column_count;
      }
      
      int* getData()
      {
	  return data;
      }
      
      void setSubmatrix(int start_row, int start_column, Matrix* m)
      {
	  for(int i = 0; i < m->getRowCount(); i++)
	    for(int j = 0; j < m->getColumnCount(); j++)
	    {
		this->set(i + start_row, j + start_column, m->get(i, j));
	    }
      }
      
      void print()
      {
	  for(int i = 0; i < row_count; i++)
	  {
	      for(int j = 0; j < column_count; j++)
		  cerr<<get(i, j)<<" ";
	      
	      cerr<<endl;
	  }
	  
	  cerr<<flush;
      }
      
      ~Matrix()
      {
	  if(destroy_data)
	    delete[] data;
      }
};

