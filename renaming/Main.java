/*

Class for renaming the actual files

*/
import java.io.File;
import java.util.Date;
import java.util.HashMap;
import java.util.SortedSet;
import java.util.TreeSet;
public class Main{
	static HashMap<Date, File> files = new HashMap<Date, File>();
	public static void main(String[] args){
		try{
			String path = args[0];
			File vidDirectory = new File(path);
			for (File video : vidDirectory.listFiles()){
				Date lastModified = new Date(video.lastModified());
				files.put(lastModified, video);
			}
			SortedSet<Date> keys = new TreeSet<Date>(files.keySet());
			System.out.println(keys);
		}catch(Exception e){

		}		
	}
}
