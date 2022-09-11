use std::{collections::HashMap, hash::Hash};

use Op::{Add, Mul};
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum Op {
    Add,
    Mul,
}

use Token::{BinOp, Int, LParen, RParen};
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Token {
    LParen,
    RParen,
    Int(i64),
    BinOp(Op),
}

impl From<char> for Token {
    fn from(c: char) -> Self {
        match c {
            '(' => LParen,
            ')' => RParen,
            '+' => BinOp(Add),
            '*' => BinOp(Mul),
            _ if c.is_digit(10) => Int((c as u8 - b'0') as i64),
            _ => unreachable!(),
        }
    }
}
pub struct ShuntingYard {
    main_stack: Vec<i64>,
    side_stack: Vec<Token>,
    priorities: HashMap<Op, i64>,
}

impl ShuntingYard {
    pub fn new(priorities: HashMap<Op, i64>) -> Self {
        Self {
            main_stack: vec![],
            side_stack: vec![],
            priorities,
        }
    }
    pub fn eval(&mut self, tokens: &[Token]) -> i64 {
        for &token in tokens {
            match token {
                LParen => self.side_stack.push(token),
                RParen => {
                    self.resolve_operations(i64::MIN);
                    assert!(
                        self.side_stack.pop() == Some(LParen),
                        "Unmatched right parenthesis"
                    );
                }
                Int(n) => self.main_stack.push(n),
                BinOp(op) => {
                    self.resolve_operations(self.priorities[&op]);
                    self.side_stack.push(token);
                }
            }
        }
        self.resolve_operations(i64::MIN);
        assert!(self.main_stack.len() == 1);
        assert!(self.side_stack.is_empty());
        self.main_stack.pop().unwrap()
    }

    // All operations assumed to be left-associative
    fn resolve_operations(&mut self, min_priority: i64) {
        while let Some(&BinOp(op)) = self.side_stack.last() {
            if self.priorities[&op] < min_priority {
                break;
            }
            self.side_stack.pop();
            self.apply(op);
        }
    }

    fn apply(&mut self, op: Op) {
        assert!(self.main_stack.len() >= 2);
        let b = self
            .main_stack
            .pop()
            .unwrap_or_else(|| panic!("Expecting two arguments for binary operation {:?}", op));
        let a = self
            .main_stack
            .pop()
            .unwrap_or_else(|| panic!("Expecting two arguments for binary operation {:?}", op));
        self.main_stack.push(match op {
            Add => a + b,
            Mul => a * b,
        });
    }
}
