pub struct Array {
    pub data: Vec<String>,
    pub capacity: usize,
}

impl Array {
    // создаёт пустой массив с заданной вместимостью
    pub fn new(capacity: usize) -> Self {
        Array { data: Vec::new(), capacity }
    }

    // добавить в конец
    pub fn push(&mut self, value: String) {
        if self.data.len() >= self.capacity {
            println!("Array is full");
            return;
        }
        self.data.push(value);
    }

    // вставить по индексу
    pub fn push_at(&mut self, index: usize, value: String) {
        if index > self.data.len() || self.data.len() >= self.capacity {
            println!("Bad index or full");
            return;
        }
        self.data.insert(index, value);
    }

    // удалить по индексу
    pub fn del(&mut self, index: usize) {
        if index >= self.data.len() {
            println!("Bad index");
            return;
        }
        self.data.remove(index);
    }

    // получить по индексу
    pub fn get(&self, index: usize) -> String {
        if index >= self.data.len() { return String::new(); }
        self.data[index].clone()
    }

    // заменить по индексу
    pub fn set(&mut self, index: usize, value: String) {
        if index >= self.data.len() {
            println!("Bad index");
            return;
        }
        self.data[index] = value;
    }

    // длина
    pub fn len(&self) -> usize {
        self.data.len()
    }

    // печать
    pub fn print(&self) {
        print!("[ ");
        for v in &self.data {
            print!("{} ", v);
        }
        println!("]");
    }

    // для сериализации
    pub fn join_csv(&self) -> String {
        let mut s = String::new();
        for v in &self.data {
            s.push_str(v);
            s.push(',');
        }
        s
    }

    // для загрузки из файла
    pub fn from_csv(csv: &str) -> Self {
        let mut arr = Array::new(100);
        for item in csv.split(',') {
            if !item.is_empty() {
                arr.data.push(item.to_string());
            }
        }
        arr
    }
}