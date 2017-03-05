package gr.auth.ee.dsproject.proximity.defplayers;

import java.util.ArrayList;
import java.util.Arrays;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;

public class Node81288173
{
	Node81288173 parent;
	ArrayList<Node81288173> children;
	int nodeDepth;
	int[] nodeMove;
	Board nodeBoard;
	double nodeEvaluation;
	
	public Node81288173() {
		parent = null;
		children = null;
		nodeDepth = 0;
		nodeMove = null;
		nodeBoard = null;
		nodeEvaluation = 0;
	}
	
	public Node81288173(Node81288173 parent, int nodeDepth, int[] nodeMove, Board nodeBoard) {
		//Set up a node without children
		this.parent = parent;
		children = null;
		this.nodeDepth = nodeDepth;
		this.nodeMove = nodeMove;
		this.nodeBoard = nodeBoard;
		this.nodeEvaluation = 0;
	}
	
	public double evaluate() {
		//Evaluate the Board the node contains
		for(int i = 0 ;i < ProximityUtilities.NUMBER_OF_COLUMNS; i++) {
			for(int j = 0;j < ProximityUtilities.NUMBER_OF_ROWS; j++) {
				if (nodeBoard.getTile(i, j).getPlayerId() == 1) {
					nodeEvaluation += nodeBoard.getTile(i, j).getScore();
				}
			}
		}
		return nodeEvaluation;
	}
	
	public void addChild(Node81288173 child) {
		if (children == null) {
			children = new ArrayList<Node81288173>();
		}
		children.add(child);
		
	}
	
	public Node81288173 getParent() {
		return parent;
	}
	
	public ArrayList<Node81288173> getChildren() {
		return children;
	}
	
	public int[] getNodeMove() {
		return nodeMove;
	}
	
	public Board getBoard() {
		return nodeBoard;
	}
	
	public int getNodeDepth() {
		return nodeDepth;
	}
		
	public double getNodeEvaluation() {
		return nodeEvaluation;
	}
	
	public void setNodeEvaluation(double evaluation) {
		nodeEvaluation = evaluation;
	}
  

}
