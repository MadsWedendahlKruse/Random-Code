
public class ArrayTest {
	
	public static int[] test1 = new int[3];
	
	public static int[][] test2 = new int[3][3];
	
	public static void main(String[] args) {
		
		for (int i = 0; i < 3; i++) {
			test1[i] = i+1;
		}
		
		test2[0] = test1;
		
		for (int i = 0; i < 3; i++) {
			test1[i] = i+4;
		}
		
		test2[1] = test1;
		
		for (int i = 0; i < 3; i++) {
			test1[i] = i+7;
		}
		
		test2[2] = test1;
		
		for (int i = 0; i < 3; i++) {
			
			for (int j = 0; j < 3; j++) {
				
				System.out.println(test2[i][j]);
			}
		}
	}
	
}
