package gr.auth.ee.dsproject.proximity.defplayers;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

import java.util.ArrayList;
import java.util.HashMap;


public class HeuristicPlayer implements AbstractPlayer {

	int score;
	int id;
	String name;
	int numOfTiles;
	
	//HashMap to keep track of enemy's tile Pool
	HashMap<Integer, Integer> enemyPool;
	//Weights of each choice criterion
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
		
		//Log the enemy move on the hashmap
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
	 * The evaluation is a sum of coefficients*criteriaEvaluation
	 * The criteria evaluation is a real number (0,1)
	 * The coefficients are real numbers
	 */
	double getEvaluation(Board board, int randomNumber, Tile tile){
		
		double evaluation;
		
		//Criteria of tile choice
		double numberOfAllies = numberOfAllies(board, tile); //Allied tiles
		double numberOfEnemies = numberOfEnemies(board, tile);//Enemy tiles
		double numberOfClaims = numberOfClaims(board, tile); //Neutral tiles
		double scoreToGain = scoreToGain(board, tile, randomNumber); //Score to gain from move
		
		evaluation = cAllies * numberOfAllies + cEnemies * numberOfEnemies 
				+ cClaims * numberOfClaims + cScore * scoreToGain;
		
		return evaluation;
		
	}
	
	
	/*
	 * Reseting coefficients to the "normal" state
	 */
	private void resetCoefficients() {
		cAllies = 3;
		cEnemies = 0.8;
		cClaims = -1.2;
		cScore = 7;

	}
	
	/*
	 * Making coefficients more aggressive
	 */
	private void offensiveCoefficients() {
		
		cAllies = 3;
		cEnemies = 0.8;
		cClaims = -1.2;
		cScore = 8;
	}
	
	/*
	 * Making coefficients more defensive
	 */
	private void defensiveCoefficients() {
		
		cAllies = 4;
		cEnemies = 0.8;
		cClaims = -1.2;
		cScore = 7;
	}

	/*
	 * Decides how to tune the coefficients
	 */
	private void coefficientTuning(Board board) {		
		
		/*
		 * Add score to either our or enemy player
		 * depending on whether we or he has more 
		 * higher number tiles left in the pool
		 */
		int myScore = 0;
		int enemyScore = 0;
		for (int i = 1;i < 21; i++) {
			if (board.getMyPool().get(i) > enemyPool.get(i) ) {
				myScore += i;
			} else if (board.getMyPool().get(i) < enemyPool.get(i) ) {
				enemyScore += i;
			}
		}
		
		//The advantage we need to have to change our play style
		int advantage = 30;
		int tuning = myScore - enemyScore;		
		
		/*
		 * If our score is greater than the enemy over
		 * the advantage play more aggressively
		 * otherwise be defensive
		 * In the case no one has an advantage be neutral
		 */
		if (tuning > advantage) {
			offensiveCoefficients();
		} else if (tuning < advantage){
			defensiveCoefficients();
		} else {
			resetCoefficients();
		}
	}
	
	/*
	 * Calculates the number of allies surrounding the
	 * certain tile and their respective scores to
	 * evaluate it
	 */
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
				//Will our score increase with this move
				if(neighbors[i].getScore() != 20 ) {
					selfIncrease++;
				}
			}
		}
		
		return (alliedNeighbors / 6.f ) * 0.9 + (selfIncrease / 6.f) * 0.1;
	}
	
	/*
	 * Calculates the number of enemies surrounding the
	 * certain tile and whether the enemy will increase
	 * his score if he places a tile there
	 */
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
				//Will enemy score increase?
				if(neighbors[i].getScore() != 20 ) {
					enemyIncrease++;
				}
			}
		}
		
		
		return (enemies / 6.f) * 0.6 + (enemyIncrease / 6.f) * 0.4;
		
	}
	
	/*
	 * Calculates the number of free tiles around
	 * a certain tile that could possibly claim our tile
	 */
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
	
	/*
	 * Calculate the score we will gain by
	 * claiming enemy tiles
	 */
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



























