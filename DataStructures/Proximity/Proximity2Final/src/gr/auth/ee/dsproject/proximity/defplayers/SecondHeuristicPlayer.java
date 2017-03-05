package gr.auth.ee.dsproject.proximity.defplayers;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

public class SecondHeuristicPlayer implements AbstractPlayer {

	int score;
	int id;
	String name;
	int numOfTiles;

	public SecondHeuristicPlayer (Integer pid)
	{
		id = pid;
		name = "Heuristic";
	}
	public String getName ()
	{

		return name;

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
	
		this.id = id;

	}

	public void setName (String name)
	{
		this.name = name;

	}
	
	public int[] getNextMove (Board board , int randomNumber){
		int[] nextMove= new int[3]; //Pinakas epistrofis
		//Dimiourgia 2 parallilwn arraylist gia sintetagmenes kai aksiologisi
		
		ArrayList<Integer[]> arrayCoord= new ArrayList<Integer[]>();
		ArrayList<Double> arrayEval= new ArrayList<Double>();
		//Prwti kinisi
		if(board.getOpponentsLastMove()[1]==-1){
			nextMove[0]=(int) (Math.random()*(ProximityUtilities.NUMBER_OF_COLUMNS - 2)+1);
			nextMove[1]=(int) (Math.random()*(ProximityUtilities.NUMBER_OF_ROWS - 2)+1);
			nextMove[2]=randomNumber;
			return nextMove;
		}
		Tile tempTile= new Tile();
		Double evaluation;
		Double maxEval=-1.0;
		for(int x=0; x<ProximityUtilities.NUMBER_OF_COLUMNS; x++){
			for(int y=0; y<ProximityUtilities.NUMBER_OF_ROWS; y++){
				Integer[] tempArray= new Integer[2];
				tempTile=board.getTile(x,y);
				tempArray[0]=x;
				tempArray[1]=y;
				if(tempTile.getPlayerId()==0){
					evaluation=getEvaluation(board, randomNumber, tempTile);
					arrayCoord.add(tempArray);
					arrayEval.add(evaluation);
				}
			}
		}
		int pos=0;
		for(int i=0; i<arrayEval.size(); i++){
			if(arrayEval.get(i)>maxEval){
				maxEval=arrayEval.get(i);
				pos=i;
			}
		}
		nextMove[0]=arrayCoord.get(pos)[0];
		nextMove[1]=arrayCoord.get(pos)[1];
		nextMove[2]=randomNumber;
		return nextMove;
		
	}
	
	double getEvaluation(Board board, int randomNumber, Tile tile){
		Tile[] neighArray= new Tile[6];  
		neighArray = ProximityUtilities.getNeighbors(tile.getX(), tile.getY(), board);//oi 6 geitones		
		double e1=0;
		double e2=0;
		double e3=0;
		double e4=0;
		//1o kritirio
		int numOfFriendly=0;
		for(int i=0; i<6; i++){
			if(neighArray[i]!=null){ //elegxos gia ektos oriou geitona
				if(neighArray[i].getPlayerId()==this.id){//elegxos tautotitas geitona
					numOfFriendly++;
				}
			}
		}
		if (randomNumber<15){
			if(numOfFriendly==6){
				return 100;
			}
			e1=numOfFriendly*10;
		}else{
			e1=numOfFriendly*6;
		}
		
		//2o kritirio
		int sumOfFriendly=0;
		for(int i=0; i<6; i++){
			if(neighArray[i]!=null){
				if(neighArray[i].getPlayerId()==this.id){
					sumOfFriendly+= neighArray[i].getScore();
				}
			}
		}
		if (randomNumber<8){
			e2=sumOfFriendly*15/111;
		}else{
			e2=sumOfFriendly*10/111;
		}
		
		//3o kritirio
		int sumOfStolen=0;
		for(int i=0; i<6; i++){
			if(neighArray[i]!=null){
				if((neighArray[i].getPlayerId()!=this.id)&&(neighArray[i].getPlayerId()!=0)&&(randomNumber>neighArray[i].getScore())){
					sumOfStolen+= neighArray[i].getScore();
				}
			}
		}
		if (randomNumber<8){
			e3=sumOfStolen*20/33;
		}else if(randomNumber<15){
			e3=sumOfStolen*25/75;
		}else{
			e3=sumOfStolen*39/111;
		}
		//4o kritirio
		
		for(int i=0; i<6; i++){
			if(neighArray[i]!=null){
				if((neighArray[i].getPlayerId()!=this.id)&&(neighArray[i].getPlayerId()!=0)&&(randomNumber>neighArray[i].getScore())){
					int numOfEmpty=0;
					int numOfNeigh=0;
					Tile[] secondNeighArray= new Tile[6]; //pinakas geitonikwn tou geitona
					secondNeighArray = ProximityUtilities.getNeighbors(neighArray[i].getX(),neighArray[i].getY(),board);
					for(int j=0; j<6; j++){
						if(secondNeighArray[j]!=null){
							if(secondNeighArray[j].getPlayerId()==0){
								numOfEmpty++;
							}
						}
					}
					numOfEmpty--;//den lambano to ypopsifio plakidio san keno
					numOfNeigh=6-numOfEmpty;
					e4+=numOfNeigh*15/6;
				}
			}
		}
		return(e1+e2+e3+e4);
	}
}