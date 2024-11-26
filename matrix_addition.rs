use std::io;

fn main() {
    let mut input = String::new();
    
    println!("첫 번째 행렬의 행과 열의 크기를 입력하세요 (공백으로 구분): ");
    io::stdin().read_line(&mut input).unwrap();
    let dims: Vec<usize> = input.trim().split_whitespace()
                                .map(|x| x.parse().unwrap())
                                .collect();
    let (rows1, cols1) = (dims[0], dims[1]);
    input.clear();

    let mut mat1: Vec<Vec<i32>> = vec![vec![0; cols1]; rows1];
    println!("첫 번째 행렬의 원소를 입력하세요: ");
    for i in 0..rows1 {
        io::stdin().read_line(&mut input).unwrap();
        mat1[i] = input.trim().split_whitespace()
                       .map(|x| x.parse().unwrap())
                       .collect();
        input.clear();
    }

    println!("두 번째 행렬의 행과 열의 크기를 입력하세요 (공백으로 구분): ");
    io::stdin().read_line(&mut input).unwrap();
    let dims: Vec<usize> = input.trim().split_whitespace()
                                .map(|x| x.parse().unwrap())
                                .collect();
    let (rows2, cols2) = (dims[0], dims[1]);
    input.clear();

    if rows1 != rows2 || cols1 != cols2 {
        println!("행렬의 크기가 다릅니다. 덧셈 불가.");
        return;
    }

    let mut mat2: Vec<Vec<i32>> = vec![vec![0; cols2]; rows2];
    println!("두 번째 행렬의 원소를 입력하세요: ");
    for i in 0..rows2 {
        io::stdin().read_line(&mut input).unwrap();
        mat2[i] = input.trim().split_whitespace()
                       .map(|x| x.parse().unwrap())
                       .collect();
        input.clear();
    }

    let mut result: Vec<Vec<i32>> = vec![vec![0; cols1]; rows1];
    for i in 0..rows1 {
        for j in 0..cols1 {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }

    println!("행렬의 합:");
    for row in result {
        println!("{:?}", row);
    }
}
