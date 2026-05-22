use std::io;
use std::collections::HashMap;

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}


enum Token {
    Series,
    Parallel,
    Value(f32),
}

// Write an answer using println!("message...");
// To debug: eprintln!("Debug message...");
fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let n = parse_input!(input_line, i32);

    let mut resistors = HashMap::new(); // Store the resistors as name -> resistance
    for _i in 0..n as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let name = inputs[0].trim().to_string();
        let r = parse_input!(inputs[1], f32);
        resistors.insert(name, r);
    }

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let circuit = input_line.trim_matches('\n');

    let result = calculate_resistance(circuit, &resistors);

    println!("{:.1}", result);
}

fn calculate_resistance(circuit: &str, resistors: &HashMap<String, f32>) -> f32 {
    let mut stack: Vec<Token> = Vec::new();
    for name in circuit.split(' ') {
        match name {
            ")" | "]" => {
                let mut values: Vec<f32> = Vec::new();
                let result = loop {
                    match stack.pop() {
                        Some(Token::Value(x)) => values.push(x),
                        Some(Token::Series) => {
                            if name != ")" { 
                                panic!("Invalid circuit: mismach of series and parallel tokens")
                            }
                            break values.iter().sum::<f32>()
                        },
                        Some(Token::Parallel) => {
                            if name != "]" { 
                                panic!("Invalid circuit: mismach of series and parallel tokens")
                            }
                            break 1.0 / (values.iter().map(|x| 1.0/x).sum::<f32>())
                        },
                        None => panic!("Stack empty, is the circuit valid?")
                    }
                };
                stack.push(Token::Value(result));
            },
            "(" => { stack.push(Token::Series); },
            "[" => { stack.push(Token::Parallel); },
            _ => {
                let resistance = match resistors.get(name) {
                    Some(r) => r,
                    None => panic!("No resistor named {:?}", name),
                };
                stack.push(Token::Value(*resistance))
            }
        }
    }

    match stack.pop() {
        Some(Token::Value(x)) => x,
        _ => panic!("Something went wrong, is the circuit valid?")
    }
}
