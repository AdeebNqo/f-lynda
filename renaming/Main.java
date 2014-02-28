/*

Class for renaming the actual files

*/
import java.io.File;
public class Main{
	public static void main(String[] args){
		try{
			String path = args[0];
			File vidDirectory = new File(path);
			for (File video : vidDirectory.listFiles()){
				System.out.println(video);
			}	
		}catch(Exception e){

		}		
	}
}
