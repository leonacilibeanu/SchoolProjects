#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

#include <mpi.h>

#define SET_TYPE_MANDELBROT 0
#define SET_TYPE_JULIA 1

#define NUM_COLORS 256

#define CUSTOM_TAG_DOUBLES 1
#define CUSTOM_TAG_INTS 2
#define CUSTOM_TAG_PARTIAL_RESULT 3

using namespace std;

class Matrix
{
    private:
      int row_count;
      int column_count;
      int* data;
      
    public:
      Matrix(int row_count, int column_count)
      {
	  this->row_count = row_count;
	  this->column_count = column_count;
	
	  data = new int[row_count * column_count];
      }
      
      //foloseste data direct, nu ii face o copie insa il va distruge cand va fi distrusa matricea
      Matrix(int row_count, int column_count, int* data)
      {
	  this->row_count = row_count;
	  this->column_count = column_count;

	  this->data = data;
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
      
      ~Matrix()
      {
	  delete[] data;
      }
};

Matrix* compute_mandelbrot(double start_x, double start_y, int num_steps_x, int num_steps_y, double resolution, int MAX_STEPS)
{
    Matrix* result = new Matrix(num_steps_x, num_steps_y);

    for(int x = 0; x < num_steps_x; x++)
      for(int y = 0; y < num_steps_y; y++)
      {
	  int step = 0;
	  
	  double c_re = start_x + resolution * x;
	  double c_im = start_y + resolution * y;
	  
	  double z_re = 0;
	  double z_im = 0;
	  
	  while(sqrt(z_re * z_re + z_im * z_im) < 2 && step < MAX_STEPS)
	  {
	      double new_z_re = z_re * z_re - z_im * z_im + c_re;
	      double new_z_im = 2 * z_re * z_im + c_im;

	      z_re = new_z_re;
	      z_im = new_z_im;

	      step++;
	  }
	  
	  int color = step % NUM_COLORS;
	  
	  result->set(x, y, color);
      }
      
    return result;
}

Matrix* compute_julia(double start_x, double start_y, int num_steps_x, int num_steps_y, double resolution, double c_re, double c_im, int MAX_STEPS)
{
    Matrix* result = new Matrix(num_steps_x, num_steps_y);
    
    for(int x = 0; x < num_steps_x; x++)
      for(int y = 0; y < num_steps_y; y++)
      {
	  int step = 0;
	  
	  double z_re = start_x + resolution * x;
	  double z_im = start_y + resolution * y;
	  
	  while(sqrt(z_re * z_re + z_im * z_im) < 2 && step < MAX_STEPS)
	  {
	      double new_z_re = z_re * z_re - z_im * z_im + c_re;
	      double new_z_im = 2 * z_re * z_im + c_im;

	      z_re = new_z_re;
	      z_im = new_z_im;
	      
	      step++;
	  }
	  
	  int color = step % NUM_COLORS;
	  
	  result->set(x, y, color);
      }
      
    return result;
}

void write_pgm(char* filename, Matrix* m)
{
    fstream fout;
    fout.open(filename, fstream::out);
    
    fout<<"P2"<<endl;	//numarul magic pt. formatul PGM
    fout<<m->getRowCount()<<" "<<m->getColumnCount()<<endl; //latimea si lungimea
    fout<<(NUM_COLORS - 1); //valoarea maxima de gri
    
    for(int column = 0; column < m->getColumnCount(); column++)
    {
	fout<<endl;

	for(int row = 0; row < m->getRowCount(); row++)  
	{
	    fout<<m->get(row, m->getColumnCount() - column - 1);
	    
	    if(column < m->getColumnCount() - 1)
		fout<<" ";
	}
      
    }
    
    fout.close();
}

int main(int argc, char* argv[])
{
    int set_type;
    double x_min, x_max, y_min, y_max;
    double resolution;
    int MAX_STEPS;
    double c_re = 0, c_im = 0;
    
    int rank, size;
    MPI_Status status;
    
    if(argc != 3)
    {
	cout<<endl<<"Usage: "<<argv[0]<<" fisier_intrare fisier_iesire"<<endl;
	return 0;
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if(rank == 0)
    {
	//fisierul de intrare este citit doar de procesul 0, care este coordonator
	fstream fin;
	fin.open(argv[1], fstream::in);
	
	fin>>set_type;
	
	fin>>x_min;
	fin>>x_max;
	fin>>y_min;
	fin>>y_max;
      
	fin>>resolution;
	
	fin>>MAX_STEPS;
	
	if(set_type == SET_TYPE_JULIA)
	{
	  fin>>c_re;
	  fin>>c_im;
	}
	
	fin.close();
	
	//trimitem informatiile catre toate celelalte procese
	for(int i = 1; i < size; i++)
	{
	    double double_buf[] = {x_min, x_max, y_min, y_max, resolution, c_re, c_im};
	    MPI_Send(double_buf, sizeof(double_buf), MPI_DOUBLE, i, CUSTOM_TAG_DOUBLES, MPI_COMM_WORLD);
	    
	    int int_buf[] = {set_type, MAX_STEPS};
	    MPI_Send(int_buf, sizeof(int_buf), MPI_INT, i, CUSTOM_TAG_INTS, MPI_COMM_WORLD);
	}
    }
    else
    {
	//primim datele de la procesul coordonator
      
	double double_buf[7];
	MPI_Recv(&double_buf[0], sizeof(double_buf), MPI_DOUBLE, 0, CUSTOM_TAG_DOUBLES, MPI_COMM_WORLD, &status);
	
	int int_buf[2];
	MPI_Recv(&int_buf[0], sizeof(int_buf), MPI_INT, 0, CUSTOM_TAG_INTS, MPI_COMM_WORLD, &status);
	
	x_min = double_buf[0];
	x_max = double_buf[1];
	y_min = double_buf[2];
	y_max = double_buf[3];
	resolution = double_buf[4];
	c_re = double_buf[5];
	c_im = double_buf[6];
	
	set_type = int_buf[0];
	MAX_STEPS = int_buf[1];
    }
    
    //fiecare proces isi calculeaza portiunea din problema pe care o rezolva, impartind planul in fasii pe axa x
    int total_steps_x = floor((x_max - x_min) / resolution);
    int total_steps_y = floor((y_max - y_min) / resolution);
    
    int start_steps_x;
    
    if(total_steps_x % size != 0)
	start_steps_x = rank * (total_steps_x / size + 1);
    else
	start_steps_x = rank * (total_steps_x / size);
    
    double start_x = x_min + start_steps_x * resolution;
    
    int num_steps_x;
    
    if(total_steps_x % size != 0)
	num_steps_x = total_steps_x / size + 1;
    else
	num_steps_x = total_steps_x / size;
    
    if(num_steps_x > total_steps_x - start_steps_x)
	num_steps_x = total_steps_x - start_steps_x;
    
    //cout<<endl<<"rank "<<rank<<" ["<<start_steps_x<<", +"<<num_steps_x<<"]"<<flush;
    //cout<<endl<<"Rank "<<rank<<": "<<start_x<<", "<<y_min<<", "<<num_steps_x<<", "<<total_steps_y<<", "<<resolution<<", "<<MAX_STEPS<<endl<<flush;
	
    Matrix* partialResult;
    
    if(set_type == SET_TYPE_JULIA)
	partialResult = compute_julia(start_x, y_min, num_steps_x, total_steps_y, resolution, c_re, c_im, MAX_STEPS);
    else
	partialResult = compute_mandelbrot(start_x, y_min, num_steps_x, total_steps_y, resolution, MAX_STEPS);

    //partialResult = new Matrix(num_steps_x, total_steps_y);
    //cout<<endl<<"Rank "<<rank<<": done"<<flush;
    
    if(rank != 0)
    {
	//trimitem rezultatele partiale catre procesul coordonator
	MPI_Send(partialResult->getData(), partialResult->getRowCount() * partialResult->getColumnCount(), MPI_INT, 0, CUSTOM_TAG_PARTIAL_RESULT, MPI_COMM_WORLD);
    
	delete partialResult;
    }
    else
    {
	//coordonatorul centralizeaza rezultatele si scrie fisierul de iesire
	
	Matrix* result = new Matrix(total_steps_x, total_steps_y);
	
	result->setSubmatrix(0, 0, partialResult);

	delete partialResult;
	
	for(int i = 1; i < size; i++)
	{
	    int* int_buf = new int[num_steps_x * total_steps_y];
	
	    MPI_Recv(&int_buf[0], num_steps_x * total_steps_y, MPI_INT, i, CUSTOM_TAG_PARTIAL_RESULT, MPI_COMM_WORLD, &status);

	    int partial_value_count = 0;
	    MPI_Get_count(&status, MPI_INT, &partial_value_count);
	    
	    partialResult = new Matrix(partial_value_count / total_steps_y, total_steps_y, int_buf);
	    
	    result->setSubmatrix(i * num_steps_x, 0, partialResult);
	    
	    delete partialResult;
	}
	
	write_pgm(argv[2], result);
	
	delete result;
    }
    
    //cout<<endl<<"Rank "<<rank<<" finished"<<flush;
    
//    Matrix* m = compute_mandelbrot(x_min, y_min, floor((x_max - x_min) / resolution), floor((y_max - y_min) / resolution), resolution, MAX_STEPS);
//    Matrix* m = compute_julia(x_min, y_min, floor((x_max - x_min) / resolution), floor((y_max - y_min) / resolution), resolution, c_re, c_im, MAX_STEPS);
    
    MPI_Finalize();
    
    return 0;
}