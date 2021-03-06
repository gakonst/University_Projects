/*	Graikos Alexandros 8128 - 6979165364 - graikosa@auth.gr
 * 	Konstantopoulos Georgios 8173 - 6978832328 - georgkonst@auth.gr
 * 	
 * 
 * 	Ergasia Domwn Dedomenwn, A Meros  
 * 
 * 
 */




package gr.auth.ee.dsproject.proximity.defplayers;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximitytUtilities;

/*
 * Klash RandomPlayer ylopoiei enan paikti o opoios epilegei mesa apo tixaia diadikasia
 * thn epomenh tou kinisi me tin methodo getNextMove
 * Me th methodo getNeighborsCoordinates dexetai ena Tile tou Board kai epistrefei tis
 * sintetagmenes twn geitonikwn Tiles
 * id: To kleidi tou paikti (1 Þ 2)
 * name: To onoma tou paikti
 * score: To score tou paikti
 * numOfTiles: To plithos twn Tiles me to kleidi tou paikti
 * 
 * 
 */
public class RandomPlayer implements AbstractPlayer
{
	//Player's ID
	private int id;
	//Player's name
	private String name;
	//Player's added tile score
	private int score;
	//Player's number of tiles owned
	private int numOfTiles;
	
	//Setters
	
	public void setId(int id) {
		this.id = id;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public void setScore(int score) {
		this.score = score;
	}
	
	public void setNumOfTiles(int numOfTiles) {
		this.numOfTiles = numOfTiles;
	}
	
	//Getters
	
	public int getId() {
		return id;
	}
	
	public String getName() {
		return name;
	}
	
	public int getScore() {
		return score;
	}
	
	public int getNumOfTiles() {
		return numOfTiles;
	}
	
	//0 arguments constructor
	
	public RandomPlayer() {
		id = 0;
		name = " ";
		score = 0;
		numOfTiles = 0;
	}
	
	//ID constructor
	
	public RandomPlayer(Integer pid) {
		id = pid.intValue();
		name = " ";
		score = 0;
		numOfTiles = 0;
	}
	
	//Full constructor
	
	public RandomPlayer(Integer pid, String pName, int pScore, int pNumOfTiles) {
		id = pid.intValue();
		name = pName;
		score = pScore;
		numOfTiles = pNumOfTiles;
	}
	
	//Returns the tile chosen for the next move
	/*
	 * Epistrefei me tixaio tropo ena pinaka 1x2 me tis sintetagmenes enos tixaiws
	 * epilegmenou Tile pou apotelei tin epomeni kinisi tou paikti
	 * x,y: Oi 2 tixaies sintatagmenes
	 * flag: Elegxei tin eksodo apo thn epanalipsi, "sikwnetai" otan vrethei diathesimo Tile
	 * tile: Oi sintetagmenes pou epistrefontai
	 */
	
	public int[] getNextMove(Board board) {
		//Returns [x y] of the next move , chooses x,y randomly

		int x, y;
		Boolean flag = true;
		do {
			//X [0,12)
			//Y [0,10)
			x = (int) (Math.random() * (ProximitytUtilities.NUMBER_OF_COLUMNS));
			y = (int) (Math.random() * (ProximitytUtilities.NUMBER_OF_ROWS));
			
			if (board.getTile(x, y).getPlayerId() == 0) {
				flag = false;
			}
		} while(flag);
		
		int[] tile = {x, y};
				
		return tile;
		
	}
		
	
	//Returns neighboring tile coordinates
	/*
	 * Dexetai to board kai 2 sintetagmenes ws orisma. Epistrefei ena pinaka 6x2 typou int
	 * me tis sintetagmenes twn geitonikwn Tile. Ean oi geitones vriskontai ektos tou board
	 * oi sintetagmenes tous orizontai ws (-1,-1)
	 *  
	 * 		á) An i seira exei mono deikti tote [Gia sintetagmenes tou arxikou Tile (x0,y0)]:
	 * 			E : (x0+1,y0)
	 * 			SE : (x0+1,y0+1)
	 * 			SW : (x0,y0+1)
	 * 			W : (x0-1,y0)
	 * 			NW : (x0,y0-1)
	 * 			NE : (x0+1,y0-1)
	 * 		â) An i seira exei zigo deikti tote:
	 *			E : (x0+1,y0)
	 * 			SE : (x0,y0+1)
	 * 			SW : (x0-1,y0+1)
	 * 			W : (x0-1,y0)
	 * 			NW : (x0-1,y0-1)
	 * 			NE : (x0,y0-1)
	 * 
	 * xPlus: x0+1
	 * xMinus: x0-1
	 * yPlus: y0+1
	 * yMinus: y0-1
	 * neighbors: O pinakas pou periexei tous geitones
	 * 
	 */
	public static int[][] getNeighborsCoordinates(Board board, int x, int y) {
		int[][] neighbors = new int[6][2];
		
		//All possible combinations of x,y
		int xPlus = x+1;
		int xMinus = x-1;
		int yPlus = y+1;
		int yMinus = y-1;
		
		//Tile belongs on odd row
		if (y % 2 == 1) {
			neighbors[0][0] = xPlus;
			neighbors[0][1] = y;
			
			neighbors[1][0] = xPlus;
			neighbors[1][1] = yPlus;
			
			neighbors[2][0] = x;
			neighbors[2][1] = yPlus;
			
			neighbors[3][0] = xMinus;
			neighbors[3][1] = y;
			
			neighbors[4][0] = x;
			neighbors[4][1] = yMinus;
			
			neighbors[5][0] = xPlus;
			neighbors[5][1] = yMinus;
		//Tile belongs on even row
		} else {
			neighbors[0][0] = xPlus;
			neighbors[0][1] = y;
			
			neighbors[1][0] = x;
			neighbors[1][1] = yPlus;
			
			neighbors[2][0] = xMinus;
			neighbors[2][1] = yPlus;
			
			neighbors[3][0] = xMinus;
			neighbors[3][1] = y;
			
			neighbors[4][0] = xMinus;
			neighbors[4][1] = yMinus;
			
			neighbors[5][0] = x;
			neighbors[5][1] = yMinus;
		}
		
		//Checks if tiles are inside the board - Else sets them to (-1,-1)
		for (int i = 0;i < 6; i++) {
			if (!board.isInsideBoard(neighbors[i][0], neighbors[i][1])) {
				neighbors[i][0] = -1;
				neighbors[i][1] = -1;
			}
		}
		
		//Returns the array of neighboring tiles
		return neighbors;
		
	}

}
