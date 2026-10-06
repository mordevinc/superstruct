// односвязный список. используем Box, чтобы владеть следующего узла
struct FNode {
    data: String,
    next: Option<Box<FNode>>,
}

pub struct ForwardList {
    pub head: Option<Box<FNode>>,
    pub size: usize,
}

impl ForwardList {
    // пустой список
    pub fn new() -> Self {
        ForwardList { head: None, size: 0 }
    }

    // вставка в голову
    pub fn push_front(&mut self, value: String) {
        let new_node = Box::new(FNode { data: value, next: self.head.take() });
        self.head = Some(new_node);
        self.size += 1;
    }

    // вставка в хвост
    pub fn push_back(&mut self, value: String) {
        let mut cur = &mut self.head;
        while let Some(node) = cur {
            cur = &mut node.next;
        }
        *cur = Some(Box::new(FNode { data: value, next: None }));
        self.size += 1;
    }

    // вставка после узла со значением anchor
    pub fn push_after(&mut self, anchor: &str, value: String) -> bool {
        let mut cur = &mut self.head;
        while let Some(node) = cur {
            if node.data == anchor {
                let new_node = Box::new(FNode { data: value, next: node.next.take() });
                node.next = Some(new_node);
                self.size += 1;
                return true;
            }
            cur = &mut node.next;
        }
        false
    }

    // вставка перед узлом со значением anchor
    pub fn push_before(&mut self, anchor: &str, value: String) -> bool {
        // если anchor в голове - просто push_front
        if let Some(h) = &self.head {
            if h.data == anchor {
                self.push_front(value);
                return true;
            }
        }
        let mut cur = &mut self.head;
        while let Some(node) = cur {
            if let Some(next) = &node.next {
                if next.data == anchor {
                    let new_node = Box::new(FNode { data: value, next: node.next.take() });
                    node.next = Some(new_node);
                    self.size += 1;
                    return true;
                }
            }
            cur = &mut node.next;
        }
        false
    }

    // удалить голову
    pub fn del_front(&mut self) {
        if self.head.is_none() {
            println!("no");
            return;
        }
        self.head = self.head.take().unwrap().next;
        self.size -= 1;
    }

    // удалить хвост
    pub fn del_back(&mut self) {
        if self.head.is_none() {
            println!("no");
            return;
        }
        // если один элемент
        if self.head.as_ref().unwrap().next.is_none() {
            self.head = None;
            self.size = 0;
            return;
        }
        // ищем предпоследний
        let mut cur = self.head.as_mut().unwrap();
        while cur.next.as_ref().unwrap().next.is_some() {
            cur = cur.next.as_mut().unwrap();
        }
        cur.next = None;
        self.size -= 1;
    }

    // удалить узел после anchor
    pub fn del_after(&mut self, anchor: &str) -> bool {
        let mut cur = &mut self.head;
        while let Some(node) = cur {
            if node.data == anchor {
                if node.next.is_some() {
                    node.next = node.next.take().unwrap().next;
                    self.size -= 1;
                    return true;
                }
                return false;
            }
            cur = &mut node.next;
        }
        false
    }

    // удалить узел перед anchor
    pub fn del_before(&mut self, anchor: &str) -> bool {
        if self.head.is_none() { return false; }
        if self.head.as_ref().unwrap().next.is_none() { return false; }
        if self.head.as_ref().unwrap().next.as_ref().unwrap().data == anchor {
            self.head = self.head.take().unwrap().next;
            self.size -= 1;
            return true;
        }
        let mut cur = &mut self.head;
        while let Some(node) = cur {
            if let Some(next) = &node.next {
                if let Some(nextnext) = &next.next {
                    if nextnext.data == anchor {
                        node.next = node.next.take().unwrap().next;
                        self.size -= 1;
                        return true;
                    }
                }
            }
            cur = &mut node.next;
        }
        false
    }

    // удалить по значению
    pub fn del_by_value(&mut self, value: &str) {
        // если в голове
        if let Some(h) = &self.head {
            if h.data == value {
                self.del_front();
                return;
            }
        }
        let mut cur = &mut self.head;
        while let Some(node) = cur {
            if let Some(next) = &node.next {
                if next.data == value {
                    node.next = node.next.take().unwrap().next;
                    self.size -= 1;
                    return;
                }
            }
            cur = &mut node.next;
        }
        println!("not found");
    }

    // найти по значению
    pub fn find(&self, value: &str) -> bool {
        let mut cur = &self.head;
        while let Some(node) = cur {
            if node.data == value { return true; }
            cur = &node.next;
        }
        false
    }

    // вывести голову
    pub fn get_head(&self) {
        match &self.head {
            Some(h) => println!("Head: {}", h.data),
            None => println!("list is empty"),
        }
    }

    // вывести хвост
    pub fn get_tail(&self) {
        let mut cur = &self.head;
        let mut last: Option<&str> = None;
        while let Some(node) = cur {
            last = Some(&node.data);
            cur = &node.next;
        }
        match last {
            Some(v) => println!("Tail: {}", v),
            None => println!("list is empty"),
        }
    }

    // рекурсивно вывести с конца
    pub fn get_reverse(&self) {
        fn rec(node: &Option<Box<FNode>>) {
            if let Some(n) = node {
                rec(&n.next);
                println!("{}", n.data);
            }
        }
        rec(&self.head);
    }

    // печать
    pub fn print(&self) {
        println!("ForwardList:");
        let mut cur = &self.head;
        while let Some(node) = cur {
            println!("{}", node.data);
            cur = &node.next;
        }
    }

    // сериализация в "a,b,c,"
    pub fn join_csv(&self) -> String {
        let mut s = String::new();
        let mut cur = &self.head;
        while let Some(node) = cur {
            s.push_str(&node.data);
            s.push(',');
            cur = &node.next;
        }
        s
    }

    // восстановление из "a,b,c,"
    pub fn from_csv(csv: &str) -> Self {
        let mut list = ForwardList::new();
        for item in csv.split(',') {
            if !item.is_empty() {
                list.push_back(item.to_string());
            }
        }
        list
    }
}