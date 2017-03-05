package gr.auth.ee.dsproject.proximity.defplayers;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

import java.util.ArrayList;

import java.util.Scanner;
import java.util.HashMap;


public class HeuristicPlayer implements AbstractPlayer {

	int score;
	int id;
	String name;
	int numOfTiles;
	HashMap<Integer, Integer> enemyPool;
	double cAllies;
	double cEnemies;
	double cClaims;
	double cScore;

	public HeuristicPlayer (Integer pid)
	{
		id = pid;
		enemyPool = new HashMap<Integer, Integer>();
		for (int i = 1;i < 21; i++) {
			enemyPool.put(i, 3);
		}
		resetCoefficients();
	}

	public String getName ()
	{

		return "Random";

	}

	public int getNumOfTiles(){
		return numOfTiles;
	}

	public void setNumOfTiles(int tiles){
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
		// TODO Auto-generated method stub
		this.id = id;

	}

	public void setName (String name)
	{
		// TODO Auto-generated method stub
		this.name = name;

	}

	
	public int[] getNextMove (Board board , int randomNumber)
	{
		//TODO fill this function
		
		//Log the enemy move
		if (board.getOpponentsLastMove()[0] != -1) {
			int currentNumber = enemyPool.get(board.getOpponentsLastMove()[2]);
			enemyPool.put(board.getOpponentsLastMove()[2] ,currentNumber - 1);
		}
		
		
		//possibleMoves contains all the possible moves the player can make
		ArrayList<double[]> possibleMoves = new ArrayList<double[]>();
		
		//The chosen tile to be returned
		int[] chosenTile = new int[3];
		
		//Tune the coefficients for evaluation
		coefficientTuning(board);

		
		//add all the free tiles for evaluation on the arrayList
		for(int i = 0; i < ProximityUtilities.NUMBER_OF_COLUMNS; i++) {
			for(int j = 0; j < ProximityUtilities.NUMBER_OF_ROWS; j++) {
				
				//If chosen tile is empty evaluate it and add to the list
				if (board.getTile(i, j).getPlayerId() == 0) {
					double[] newMove = {i, j, 
							getEvaluation(board, randomNumber, board.getTile(i, j)) };
					possibleMoves.add(newMove);
				}
				
			}
		}
		
		
		//Find best possibleMove
		double max = possibleMoves.get(0)[2];
		chosenTile[0] = (int) possibleMoves.get(0)[0];
		chosenTile[1] = (int) possibleMoves.get(0)[1];
		
		for(int i = 1;i < possibleMoves.size(); i++) {
			if ( possibleMoves.get(i)[2] > max) {
				max = possibleMoves.get(i)[2];
				chosenTile[0] = (int) possibleMoves.get(i)[0];
				chosenTile[1] = (int) possibleMoves.get(i)[1];
			}
		}
		
		chosenTile[2] = randomNumber;
		
		
		
		return chosenTile;
		

	}
	
	/*
	 * 
	 * Console input tile choice (for debugging purposes)
	 * 
	 * 
	 * 
	public int[] getNextMove(Board board, int randomNumber) {
		Scanner in = new Scanner(System.in);
		
		int x = in.nextInt();
		int y = in.nextInt();
		Tile tile = board.getTile(x, y);
		double evaluation = getEvaluation(board, randomNumber , tile);
		
		System.out.println("Evaluation for chosen tile " + evaluation);
		
		int[] tileToReturn = {x, y, randomNumber};
		return tileToReturn;
	}
	*/
	
	
	
	double getEvaluation(Board board, int randomNumber, Tile tile){
		
		//TODO fill this function

		double evaluation;
		
		double numberOfAllies = numberOfAllies(board, tile); //Allied tiles
		double numberOfEnemies = numberOfEnemies(board, tile);//Enemy tiles
		double numberOfClaims = numberOfClaims(board, tile); //Neutral tiles
		double scoreToGain = scoreToGain(board, tile, randomNumber); //Score to gain from move
		
		//System.out.println("\n\nNumber of allies: " + numberOfAllies + "\nNumber of Claims: " + numberOfClaims
		//		+ "\nScore to Gain " + scoreToGain);
		
		
		
		evaluation = cAllies * numberOfAllies + cEnemies * numberOfEnemies 
				+ cClaims * numberOfClaims + cScore * scoreToGain;
		
		return evaluation;
		
	}
	
	private void resetCoefficients() {
		cAllies = 3;
		cEnemies = 0.8;
		cClaims = -1.2;
		cScore = 7;

	}
	
	private void offensiveCoefficients() {
		
		cAllies = 3;
		cEnemies = 0.8;
		cClaims = -1.2;
		cScore = 8;
	}
	
	private void defensiveCoefficients() {
		
		cAllies = 4;
		cEnemies = 0.8;
		cClaims = -1.2;
		cScore = 7;
	}

	private void coefficientTuning(Board board) {
		/* Tune the coefficients according to your and the enemy remaining pools
		 * 2 states
		 * Offensive/Defensive 
		 * If our claiming capabilities < enemy capabilities --> defend our territory
		 * Else --> claim enemy territory
		 */
		
		
		int myScore = 0;
		int enemyScore = 0;
		for (int i = 1;i < 21; i++) {
			if (board.getMyPool().get(i) > enemyPool.get(i) ) {
				//Add score to me
				myScore += i;
			} else if (board.getMyPool().get(i) < enemyPool.get(i) ) {
				enemyScore += i;
			}
		}
		
		int advantage = 0;
		int tuning = myScore - enemyScore;		
		

		if (tuning > advantage) {
			//Go offensive

			System.out.println("Offensive");
			
			offensiveCoefficients();

		} else if (tuning < advantage){
			//Go defensive
			

			System.out.println("Defensive");
			
			defensiveCoefficients();

		} else {
			resetCoefficients();
		}
	}
	
	private double numberOfAllies(Board board, Tile tile) {
		Tile[] neighbors = ProximityUtilities.getNeighbors(tile.getX(), tile.getY(), board);
		int alliedNeighbors = 0;
		int selfIncrease = 0;
		
		//Check all neighbors for allies
		for (int i = 0; i < neighbors.length; i++) {
			if (neighbors[i] == null) {
				continue;
			}
			if (neighbors[i].getPlayerId() == this.id) {
				alliedNeighbors++;
				if(neighbors[i].getScore() != 20 ) {
					selfIncrease++;
				}
			}
		}
		
		//System.out.println("Allied neighbors found " + alliedNeighbors);
		return (alliedNeighbors / 6.f ) * 0.9 + (selfIncrease / 6.f) * 0.1;
		
	}
	
	private double numberOfEnemies(Board board, Tile tile) {
		Tile[] neighbors = ProximityUtilities.getNeighbors(tile.getX(), tile.getY(), board);
		int enemies = 0;
		int enemyIncrease = 0;
		
		int enemyID;
		if (this.id == 1){
			enemyID = 2;
		} else {
			enemyID = 1;
		}
		
		//Check all neighbors for enemies
		for (int i = 0; i < neighbors.length; i++) {
			if (neighbors[i] == null) {
				continue;
			}
			if (neighbors[i].getPlayerId() == enemyID) {
				enemies++;
				if(neighbors[i].getScore() != 20 ) {
					enemyIncrease++;
				}
			}
		}
		
		
		return (enemies / 6.f) * 0.6 + (enemyIncrease / 6.f) * 0.4;
		
	}
	

	private double numberOfClaims(Board board, Tile tile) {
		Tile[] neighbors = ProximityUtilities.getNeighbors(tile.getX(), tile.getY(), board);
		int freeNeighbors = 0;
		
		//Check all neighbors for free tiles
		for (int i = 0; i < neighbors.length; i++) {
			if (neighbors[i] == null) {
				continue;
			}
			if (neighbors[i].getPlayerId() == 0) {
				freeNeighbors++;
			}
		}
		
		return freeNeighbors / 6.f;
	}
	
	private double scoreToGain(Board board, Tile tile, int randomNumber) {
		Tile[] neighbors = ProximityUtilities.getNeighbors(tile.getX(), tile.getY(), board);
		int scoreToGain = 0;
		
		int enemyID;
		if (this.id == 1){
			enemyID = 2;
		} else {
			enemyID = 1;
		}
		
		//Check all neighbors for enemies
		for (int i = 0; i < neighbors.length; i++) {
			if (neighbors[i] == null) {
				continue;
			}
			
			//If is enemy tile and its score < my score
			if (neighbors[i].getPlayerId() == enemyID) {
				if (neighbors[i].getScore() < randomNumber) {
					scoreToGain += neighbors[i].getScore();
				}
			}
		}
		
		return scoreToGain / 114.f;
		
	}
	

}



























