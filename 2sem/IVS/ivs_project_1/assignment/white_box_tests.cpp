//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     TOMAS LAPSANSKY <xlapsa00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author TOMAS LAPSANSKY
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class Construktor : public ::testing::Test
{

protected:
	
};

class Operations : public ::testing::Test
{
	
protected:

};

class Solving : public ::testing::Test
{
	
protected:

};

class Empty_Operations : public ::testing::Test
{
	
protected:
	
};

TEST_F(Construktor, ZeroConstruktor)
{
	Matrix *ZeroMatrix = new Matrix();
	
	EXPECT_EQ(ZeroMatrix->get(0, 0), 0);
	EXPECT_TRUE(std::isnan(ZeroMatrix->get(0, 1)));
	EXPECT_TRUE(std::isnan(ZeroMatrix->get(1, 0)));
	
	ZeroMatrix->~Matrix();
}

TEST_F(Construktor, BadConstruktor)
{
	EXPECT_THROW(Matrix *Matrix1 = new Matrix(0, 2), void*);
	EXPECT_THROW(Matrix *Matrix2 = new Matrix(2, 0), void*);
}

TEST_F(Construktor, Construktor_r_c)
{
	Matrix *CMatrix = new Matrix(1, 1);
	
	EXPECT_EQ(CMatrix->get(0, 0), 0);
	EXPECT_TRUE(std::isnan(CMatrix->get(0, 1)));
	EXPECT_TRUE(std::isnan(CMatrix->get(1, 0)));
	
	CMatrix = new Matrix(4, 4);
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			EXPECT_EQ(CMatrix->get(i, j), 0);
		}
	}
	EXPECT_TRUE(std::isnan(CMatrix->get(5, 4)));
	EXPECT_TRUE(std::isnan(CMatrix->get(4, 5)));
	
	CMatrix = new Matrix(3, 8);
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			EXPECT_EQ(CMatrix->get(i, j), 0);
		}
	}
	EXPECT_TRUE(std::isnan(CMatrix->get(3, 9)));
	EXPECT_TRUE(std::isnan(CMatrix->get(4, 8)));
	
	CMatrix = new Matrix(8, 3);
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			EXPECT_EQ(CMatrix->get(i, j), 0);
		}
	}
	EXPECT_TRUE(std::isnan(CMatrix->get(9, 3)));
	EXPECT_TRUE(std::isnan(CMatrix->get(8, 4)));
}

TEST_F(Empty_Operations, Set_and_Get)
{
	Matrix *omatrix = new Matrix(3, 3);
	
	double SetMatrix[3][3] =	{
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			EXPECT_TRUE(omatrix->set(r, c, SetMatrix[r][c]));
			ASSERT_EQ(omatrix->get(r, c), SetMatrix[r][c]);
		}
	}
	
	EXPECT_FALSE(omatrix->set(4, 3, 10));
	EXPECT_FALSE(omatrix->set(3, 4, 10));
}

TEST_F(Operations, Set_Vector_1)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
								
								
	Matrix *matrix = new Matrix(3, 3);
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix->set(r, c, SetupMatrix1[r][c]);
		}
	}
	
	EXPECT_TRUE(matrix->set({{10, 10, 10}, {10, 10, 10}, {10, 10, 10}}));
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
			EXPECT_EQ(matrix->get(r, c), 10);
	}
}

TEST_F(Operations, Set_Vector_2)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
								
								
	Matrix *matrix = new Matrix(3, 3);
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix->set(r, c, SetupMatrix1[r][c]);
		}
	}
	
	EXPECT_TRUE(matrix->set({{10, 10}, {10, 10}, {10, 10}}));
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 2; c++)
			EXPECT_EQ(matrix->get(r, c), 10);
	}
		EXPECT_EQ(matrix->get(0, 2), 0); //????????????????????????????
		EXPECT_EQ(matrix->get(1, 2), 0);
		EXPECT_EQ(matrix->get(2, 2), 0);
}

TEST_F(Operations, Set_Vector_3)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
								
	Matrix *matrix = new Matrix(3, 3);
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix->set(r, c, SetupMatrix1[r][c]);
		}
	}
	
	EXPECT_FALSE(matrix->set({{10, 10, 10, 10}, {10, 10, 10, 10}, {10, 10, 10, 10}, {10, 10, 10, 10}})); //???????????????????????
}

TEST_F(Operations, Equal)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
								
	Matrix *matrix1 = new Matrix(3, 3);
	Matrix *matrix2 = new Matrix(3, 3);
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix1->set(r, c, SetupMatrix1[r][c]);
			matrix2->set(r, c, SetupMatrix1[r][c]);
		}
	}
	
	EXPECT_TRUE(*matrix1==*matrix2);
	
	Matrix *matrix3 = new Matrix(4, 4);
	EXPECT_THROW(*matrix1==*matrix3, void*);
	
	Matrix *matrix4 = new Matrix(3, 3);
	EXPECT_FALSE(*matrix1==*matrix4);
}

TEST_F(Operations, Plus)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
								
	Matrix *matrix1 = new Matrix(3, 3);
	Matrix *matrix2 = new Matrix(3, 3);
	Matrix *matrix3 = new Matrix(3, 3);
	Matrix *matrix4 = new Matrix(3, 3);
	Matrix *matrix5 = new Matrix(4, 3);
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix1->set(r, c, SetupMatrix1[r][c]);
			matrix3->set(r, c, SetupMatrix1[r][c]);
			matrix4->set(r, c, 2*SetupMatrix1[r][c]);
		}
	}
	
	EXPECT_TRUE((*matrix1+*matrix2)==*matrix1);
	EXPECT_TRUE((*matrix1+*matrix3)==*matrix4);
	EXPECT_THROW(*matrix1+*matrix5, void*);
}

TEST_F(Operations, Multiple_Matrix)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
	double SetupMatrix2[3][3] = {
								{30, 36, 42},
								{66, 81, 96},
								{102, 126, 150}
								};
								
	Matrix *matrix1 = new Matrix(3, 3);
	Matrix *matrix2 = new Matrix(3, 3);
	Matrix *matrix3 = new Matrix(4, 4);
	Matrix *matrix4 = new Matrix(3, 3);
	
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix1->set(r, c, SetupMatrix1[r][c]);
			matrix4->set(r, c, SetupMatrix2[r][c]);
		}
	}
	
	EXPECT_TRUE((*matrix1)*(*matrix2)==(*matrix2));
	EXPECT_THROW(((*matrix1)*(*matrix3)==(*matrix2)), void*);
	EXPECT_TRUE((*matrix1)*(*matrix1)==(*matrix4));
}

TEST_F(Operations, Multiple_Constant)
{
	double SetupMatrix1[3][3] = {
								{1, 2, 3},
								{4, 5, 6},
								{7, 8, 9}
								};
	double SetupMatrix2[3][3] = {
								{2, 4, 6},
								{8, 10, 12},
								{14, 16, 18}
								};
								
	Matrix *matrix1 = new Matrix(3, 3);
	Matrix *matrix2 = new Matrix(3, 3);
	Matrix *matrix3 = new Matrix(3, 3);
							
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix1->set(r, c, SetupMatrix1[r][c]);
			matrix2->set(r, c, SetupMatrix2[r][c]);
		}
	}
	
	EXPECT_TRUE((*matrix1)*1==(*matrix1));
	EXPECT_TRUE((*matrix1)*0==(*matrix3));
	EXPECT_TRUE((*matrix1)*(-2)==(*matrix2)*(-1));
}

TEST_F(Solving, Solve_Equation)
{
	double SetupMatrix1[3][3] =	{
								{1, 2, 3},
								{3, 2, 1},
								{3, 1, 2}
								};
								
	double SetupMatrix2[3][3] =	{
								{1, 2, 3},
								{2, 4, 6},
								{4, 8, 12}
								};
								
	Matrix *matrix1 = new Matrix(3, 3);
	Matrix *matrix2 = new Matrix(4, 3);
	Matrix *matrix3 = new Matrix(4, 4);
	Matrix *matrix2x2 = new Matrix(2, 2);
	Matrix *matrix1x1 = new Matrix(1, 1);
	matrix1x1->set(0, 0, 2);
	Matrix *matrixSingular = new Matrix(3, 3);
	std::vector<double> b1 = {14, 10, 11};
	std::vector<double> b2 = {0, 0};
	std::vector<double> b3 = {2};
	std::vector<double> result1 = {1, 2, 3};
	std::vector<double> result2 = {0, 0};
	std::vector<double> result3 = {0};
	
								
	for(int r = 0; r < 3; r++)
	{
		for(int c = 0; c < 3; c++)
		{
			matrix1->set(r, c, SetupMatrix1[r][c]);
			matrixSingular->set(r, c, SetupMatrix2[r][c]);
		}
	}
	
	for(int r = 0; r < 2; r++)
	{
		for(int c = 0; c < 2; c++)
		{
			matrix2x2->set(r, c, SetupMatrix1[r][c]);
		}
	}
	
	EXPECT_TRUE((matrix1->solveEquation(b1))==(result1));
	EXPECT_THROW((matrix2->solveEquation(b1))==(result1), void*);
	EXPECT_THROW((matrix3->solveEquation(b1))==(result1), void*);
	EXPECT_THROW((matrixSingular->solveEquation(b1))==(result1), void*);
	EXPECT_TRUE((matrix2x2->solveEquation(b2))==(result2));
	EXPECT_TRUE((matrix1x1->solveEquation(b3))==(result3));

	double SetupMatrix3[4][4] =	{
								{2, 1, 1, 1},
								{1, 2, 1, 1},
								{1, 1, 2, 1},
								{1, 1, 1, 2}
								};
								
	Matrix *matrix4x4 = new Matrix(4, 4);
	std::vector<double> b4x4 = {5, 5, 5, 5};
	std::vector<double> result4x4 = {1, 1, 1, 1};
	
	for(int r = 0; r < 4; r++)
	{
		for(int c = 0; c < 4; c++)
		{
			matrix4x4->set(r, c, SetupMatrix3[r][c]);
		}
	}
	EXPECT_TRUE((matrix4x4->solveEquation(b4x4))==(result4x4));
	
}
/*** Konec souboru white_box_tests.cpp ***/
