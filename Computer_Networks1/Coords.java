import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;

public class Coords {

	
	String coordParser(float coord){
		String degMin,ssec,degminSec;
		String deg[];
		//String command;
		int sec;
		deg = String.valueOf(coord).split("\\.");
		degMin = String.valueOf(Integer.parseInt(deg[0]));
		sec = (int) (Float.parseFloat(deg[1])*0.006);
		ssec = String.valueOf(sec);
		degminSec = degMin+ssec;
		return degminSec;
	}
	
	void cleanup() throws IOException{
		BufferedReader dirty = null;
		PrintWriter cleanFile = null;
		
		try {
			 dirty = new BufferedReader(new FileReader("gps-Coords.txt"));
			 cleanFile = new PrintWriter(new FileOutputStream(new File("cleanFile.txt")));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String line;
		while ((line=dirty.readLine()) !=null){
			if (line.endsWith("TRACKING") || (!line.startsWith("$GPGGA"))){
				continue;
			}else{
				cleanFile.write(line);
				cleanFile.write(System.getProperty("line.separator"));
				}
						
		}
		dirty.close();
		cleanFile.close();
	}
	String coords(){
		BufferedReader gpsCoords = null;
		//String longitude,latitude;
		String command ="",x,y;
		int commandCount =0;
		try {
			cleanup();
		} catch (IOException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		try {
			gpsCoords = new BufferedReader(new FileReader("cleanFile.txt"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		String line = null;
        float distance = (float) 0.066;
		try {
			gpsCoords.readLine();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		float[] longitude = new float[403];
		float[] latitude = new float[403];
		for (int i=0;i<longitude.length;i++){
			try {
				line=gpsCoords.readLine();
			} catch (IOException e) {
				e.printStackTrace();
			}
 
			//if (line.endsWith("TRACKING")){
				//continue;
				//}
			String[] data=line.split(",");
			//String exit = data[0].toString();
			longitude[i] = Float.parseFloat(data[2]);
			latitude[i] = Float.parseFloat(data[4]); //E
			//if (exit.equals(exitWord))break;
			}	
		for (int i=0;i<longitude.length;i++){
			for (int j=0;j<longitude.length;j++){
				float diff1 = Math.abs(longitude[j]- longitude[i]);
				float diff2 = Math.abs(latitude[j]-latitude[i]);
				if (Math.sqrt(Math.pow(diff1, 2)+Math.pow(diff2, 2))>distance){
					//System.out.print("Longtidue: "+longitude[j]+" Latitude: "+latitude[j]);
					y = coordParser(longitude[j]);
					x = coordParser(latitude[j]);
					command+="T="+x+y;
					commandCount++;
					//System.out.print(commandCount);
					i=j;
					if (commandCount==9)break;
				}
			}
			if (commandCount==9)break;
		}
		return command;
	}	
	
	
	
	
	
	
}