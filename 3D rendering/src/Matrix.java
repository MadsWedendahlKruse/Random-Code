
public class Matrix {
	private double[][] matrix;

	public Matrix(int rows, int columns) {
		matrix = new double[rows][columns];
	}

	public double getValue(int rowIdx, int columnIdx) {
		return matrix[rowIdx][columnIdx];
	}

	public void setValue(int rowIdx, int columnIdx, double value) {
		matrix[rowIdx][columnIdx] = value;
	}

	public Vector dot(Vector vector) {

		double xResult = 0;
		double yResult = 0;
		double zResult = 0;

		for (int i = 0; i < matrix.length; i++) {

			for (int j = 0; j < matrix[0].length; j++) {
				
				double vectorVal = 0;
				
				switch (j) {
				case 0:
					vectorVal = vector.getX();
					break;
				case 1:
					vectorVal = vector.getY();
					break;
				case 2:
					vectorVal = vector.getZ();
					break;
				default:
					break;
				}

				switch (i) {
				case 0:
					xResult += matrix[i][j]*vectorVal;
					break;
				case 1:
					yResult += matrix[i][j]*vectorVal;
					break;
				case 2:
					zResult += matrix[i][j]*vectorVal;
					break;
				default:
					break;
				}
			}
		}

		return new Vector(xResult, yResult, zResult);
	}

	public double[][] getMatrix() {
		return matrix;
	}

	public void setMatrix(double[][] matrix) {
		this.matrix = matrix;
	}
}
