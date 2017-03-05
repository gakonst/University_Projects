package gr.auth.ee.dsproject.proximity.defplayers;

import java.util.ArrayList;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;
import gr.auth.ee.dsproject.proximity.defplayers.Node81288173;

public class MinMaxPlayer implements AbstractPlayer
{

  int score;
  int id;
  String name;
  int numOfTiles;

  public MinMaxPlayer (Integer pid)
  {
    id = pid;
    name = "MinMaxPlayer";
  }

  public String getName ()
  {

    return name;

  }

  public int getNumOfTiles ()
  {
    return numOfTiles;
  }

  public void setNumOfTiles (int tiles)
  {
    numOfTiles = tiles;
  }

  public int getId ()
  {
    return id;
  }

  public void setScore (int score)
  {
    this.score = score;
  }

  public int getScore ()
  {
    return score;
  }

  public void setId (int id)
  {

    this.id = id;

  }

  public void setName (String name)
  {
    this.name = name;

  }

  public int[] getNextMove (Board board, int randomNumber)
  {
	  //Create a root for out MinMax tree
	  int[] emptyArray = {-1, -1, randomNumber};
	  Node81288173 root = new Node81288173(null, 0, emptyArray, board);
	  
	  //Create the tree that spans beneath the root
	  createMySubTree(root, 1);
	  
	  //Apply minmax algorithm to the tree
	  int index = chooseMinMaxMove(root);
	  
	  //Return the value that was given to the root by the minmax algorithm
	  return root.getChildren().get(index).getNodeMove();

  }

  public void createMySubTree (Node81288173 parent, int depth)
  {
	  //The possible moves will be on the empty tiles of the parent's board
	  Board parentBoard = parent.getBoard();
	  
	  for(int i = 0; i < ProximityUtilities.NUMBER_OF_COLUMNS; i++) {
		  for(int j = 0; j < ProximityUtilities.NUMBER_OF_ROWS; j++) {
			  //If the tile is empty create a new child Node that contains that possible move
			  if (parentBoard.getTile(i, j).getPlayerId() == 0) { 
				  int[] nodeMove = {i, j, parent.getNodeMove()[2]};
				  Node81288173 child = new Node81288173(parent, depth, nodeMove,
						   ProximityUtilities.boardAfterMove(this.id, parentBoard, nodeMove));
				  parent.addChild(child);
				  createOpponentSubtree(child, depth + 1);
			  }
		  }
	  }

  }

  public void createOpponentSubtree (Node81288173 parent, int depth)
  {
	  Board parentBoard = parent.getBoard();
	  
	  for(int i = 0; i < ProximityUtilities.NUMBER_OF_COLUMNS; i++) {
		  for(int j = 0; j < ProximityUtilities.NUMBER_OF_ROWS; j++) {
			  //If the tile is empty create a new child Node that contains the enemy possible move
			  if (parentBoard.getTile(i, j).getPlayerId() == 0) {
				  //Choose enemy number that will be played
				  int enemyRandomNumber = 20;
				  
				  int enemyID;
				  if (this.id == 1) {
					  enemyID = 2;
				  } else {
					  enemyID = 1;
				  }
				  
				  int[] nodeMove = {i, j, enemyRandomNumber};
				  Node81288173 child = new Node81288173(parent, depth, nodeMove,
						 ProximityUtilities.boardAfterMove(enemyID, parentBoard, nodeMove));
				  parent.addChild(child);
			  }
		  }
	  }

  }
  
  int chooseMinMaxMove(Node81288173 node) {
	  //The index of the child(move) we will be returning 
	  int childIndex = 0;;
	  
	  //If node given is a leaf
	  if (node.getChildren() == null) {
		  node.evaluate();
	  /*If node is not a leaf do the same for each of its children and put the min/max value 
	  *depending on the node depth
	  */
	  } else {
		  //Evaluate each of the children nodes
		  for (int i = 0;i < node.getChildren().size(); i++) {
			  chooseMinMaxMove(node.getChildren().get(i));
		  }

		  
		  //Get their min or max value
		  
		  //If it is our move get max value of children
		  if (node.getNodeDepth() % 2 == 0) {
			  double max = node.getChildren().get(0).getNodeEvaluation();
			  childIndex = 0;
			  for(int i = 1; i < node.getChildren().size(); i++) {
				  double potentialMax = node.getChildren().get(i).getNodeEvaluation();
				  if (potentialMax > max) {
					  max = potentialMax;
					  childIndex = i;
				  }
			  }
			  node.setNodeEvaluation(max);
		  //If it is enemy's move get min value of children
		  } else {
			  double min = node.getChildren().get(0).getNodeEvaluation();
			  for(int i = 1; i < node.getChildren().size(); i++) {
				  double potentialMin = node.getChildren().get(i).getNodeEvaluation();
				  childIndex = 0;
				  if (potentialMin < min) {
					  min = potentialMin;
					  childIndex = i;
				  }
			  }
			  node.setNodeEvaluation(min);
		  }
	  }
	  
	  return childIndex;
	  
  }



}
