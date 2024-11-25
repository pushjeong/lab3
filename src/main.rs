use std::io;

fn main() {
    let mut input = String::new();

    // 행렬의 크기 입력받기
    println!("Enter the number of rows and columns for the matrices:");
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let dims: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|x| x.parse::<usize>().expect("Please enter valid integers"))
        .collect();

    if dims.len() != 2 {
        println!("Please enter exactly two integers for rows and columns.");
        return;
    }

    let rows = dims[0];
    let cols = dims[1];

    // 두 행렬 초기화
    println!("Enter the elements of the first matrix:");
    let matrix1 = read_matrix(rows, cols);

    println!("Enter the elements of the second matrix:");
    let matrix2 = read_matrix(rows, cols);

    // 행렬 덧셈
    let result = add_matrices(&matrix1, &matrix2, rows, cols);

    // 결과 출력
    println!("The resulting matrix is:");
    print_matrix(&result, rows, cols);
}

fn read_matrix(rows: usize, cols: usize) -> Vec<i32> {
    let mut matrix = Vec::new();
    let mut input = String::new();

    for i in 0..rows {
        println!("Enter row {} ({} elements):", i + 1, cols);
        input.clear();
        io::stdin().read_line(&mut input).expect("Failed to read input");

        let row: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse::<i32>().expect("Please enter valid integers"))
            .collect();

        if row.len() != cols {
            panic!("Each row must have exactly {} elements.", cols);
        }

        matrix.extend(row);
    }

    matrix
}

fn add_matrices(matrix1: &Vec<i32>, matrix2: &Vec<i32>, rows: usize, cols: usize) -> Vec<i32> {
    if matrix1.len() != rows * cols || matrix2.len() != rows * cols {
        panic!("Matrix sizes do not match the specified dimensions.");
    }

    matrix1
        .iter()
        .zip(matrix2.iter())
        .map(|(a, b)| a + b)
        .collect()
}

fn print_matrix(matrix: &Vec<i32>, rows: usize, cols: usize) {
    for i in 0..rows {
        let row: Vec<_> = matrix[i * cols..(i + 1) * cols].to_vec();
        println!("{:?}", row);
    }
}

