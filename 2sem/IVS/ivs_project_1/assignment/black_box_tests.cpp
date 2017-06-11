//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     TOMAS LAPSANSKY <xlapsa00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author TOMAS LAPSANSKY
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree leaf;
};

class NonEmptyTree : public ::testing::Test
{
	virtual void SetUp()
	{
		int value[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
		
		for(int i = 0; i < 14; i++)
			leaf.InsertNode(value[i]);
	}

protected:
	BinaryTree leaf;
};

TEST_F(EmptyTree, Insert)
{
	EXPECT_TRUE(leaf.GetRoot() == NULL);

	std::pair<bool, BinaryTree::Node_t*> temp;

	temp = leaf.InsertNode(10);
		ASSERT_TRUE(std::get<0>(temp) == true);
		ASSERT_FALSE(std::get<1>(temp) == NULL);

	ASSERT_FALSE(leaf.GetRoot() == NULL);
	EXPECT_TRUE(leaf.GetRoot()->key == 10);
}

TEST_F(EmptyTree, Find)
{
	EXPECT_TRUE(leaf.FindNode(0) == NULL);
}

TEST_F(EmptyTree, Delete)
{
	EXPECT_FALSE(leaf.DeleteNode(0));
}

TEST_F(NonEmptyTree, Insert)
{
	ASSERT_FALSE(leaf.GetRoot() == NULL);

	std::pair<bool, BinaryTree::Node_t*> temp;

	temp = leaf.InsertNode(4);
		EXPECT_TRUE(std::get<0>(temp) == false);
		EXPECT_TRUE(std::get<1>(temp)->key == 4);

	temp = leaf.InsertNode(15);
		EXPECT_TRUE(std::get<0>(temp) == true);
		EXPECT_FALSE(std::get<1>(temp) == NULL);
}

TEST_F(NonEmptyTree, Find)
{
	EXPECT_TRUE(leaf.FindNode(0) == NULL);
	
	for(int i = 1; i<=14; i++)
	{
		EXPECT_FALSE(leaf.FindNode(i) == NULL);
		EXPECT_TRUE(leaf.FindNode(i)->key == i);
	}
}

TEST_F(NonEmptyTree, Delete)
{
	EXPECT_TRUE(leaf.DeleteNode(15) == false);

	for(int i = 1; i<=14; i++)
	{
		EXPECT_TRUE(leaf.DeleteNode(i) == true);
		EXPECT_TRUE(leaf.DeleteNode(i) == false);
	}
}

TEST_F(NonEmptyTree, AXIOM1)
{
	std::vector<BinaryTree::Node_t*> outLeafNodes;
	
	leaf.GetLeafNodes(outLeafNodes);
	
	for(int i = 0; i<outLeafNodes.size(); i++)
	{
		EXPECT_TRUE(outLeafNodes[i]->pLeft == NULL);
		EXPECT_TRUE(outLeafNodes[i]->pRight == NULL);
		EXPECT_TRUE(outLeafNodes[i]->color != 0);
	}
}

TEST_F(NonEmptyTree, AXIOM2)
{
	std::vector<BinaryTree::Node_t*> outAllNodes;
	
	leaf.GetAllNodes(outAllNodes);
	
	for(int i = 0; i<outAllNodes.size(); i++)
	{
		if(outAllNodes[i]->color == 0)
		{
			EXPECT_TRUE(outAllNodes[i]->pLeft->color != 0);
			EXPECT_TRUE(outAllNodes[i]->pRight->color != 0);
		}
	}
}

TEST_F(NonEmptyTree, AXIOM3)
{
	std::vector<BinaryTree::Node_t*> outLeafNodes;
	
	leaf.GetLeafNodes(outLeafNodes);
	
	BinaryTree::Node_t *tmp = outLeafNodes[0];
	int BLACK = 0;
	
	while(tmp != leaf.GetRoot())
	{
		if(tmp->color != 0)
			BLACK++;
			
		tmp = tmp->pParent;
	}
	
	for(int i = 1; i<outLeafNodes.size(); i++)
	{
		tmp = outLeafNodes[i];
		int tmpBLACK = 0;
		
		while(tmp != leaf.GetRoot())
		{
			if(tmp->color != 0)
				tmpBLACK++;
				
			tmp = tmp->pParent;
		}
		
		EXPECT_EQ(BLACK, tmpBLACK);
	}
}

/*** Konec souboru black_box_tests.cpp ***/
