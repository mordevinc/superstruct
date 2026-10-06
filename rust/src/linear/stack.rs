pub struct Stack {
    pub data: Vec<String>,
}

impl Stack {
    // пустой стек
    pub fn new() -> Self {
        Stack { data: Vec::new() }
    }

    // положить на вершину
    pub fn push(&mut self, value: String) {
        self.data.push(value);
    }

    // снять с вершины
    pub fn pop(&mut self) -> String {
        match self.data.pop() {
            Some(v) => v,
            None => {
                println!("no");
                String::new()
            }
        }
    }

    // печать: сверху вниз
    pub fn print(&self) {
        println!("Stack (top -> bottom):");
        for v in self.data.iter().rev() {
            println!("{}", v);
        }
    }

    // сериализация: порядок top->bottom, как в C++
    pub fn join_csv(&self) -> String {
        let mut s = String::new();
        for v in self.data.iter().rev() {
            s.push_str(v);
            s.push(',');
        }
        s
    }

    // из csv: читаем в порядке top->bottom, но кладём в обратном
    pub fn from_csv(csv: &str) -> Self {
        let mut stack = Stack::new();
        let items: Vec<&str> = csv.split(',').filter(|s| !s.is_empty()).collect();
        // items[0] — вершина стека, значит кладём в обратном порядке
        for item in items.iter().rev() {
            stack.data.push(item.to_string());
        }
        stack
    }
}