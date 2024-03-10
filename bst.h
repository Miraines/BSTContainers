#include <memory>
#include <functional>

// Определение тегов для различных видов обхода
struct InOrder {};
struct PreOrder {};
struct PostOrder {};

template<typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
class BinarySearchTree {
 public:
  // Определение типов для удобства
  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

 private:
  // Определение узла дерева
  struct Node {
    value_type value;
    Node* left;
    Node* right;
    Node* parent;
    Node(const value_type& val, Node* parent = nullptr)
        : value(val), parent(parent), left(nullptr), right(nullptr) {}
  };
  using NodeAllocator = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
  NodeAllocator node_allocator_;
  Node* root; // Указатель на корень дерева

 public:
  // Определение итератора
  template<typename Order>
  class iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    // Конструктор
    iterator(Node* node = nullptr) : node(node) {}

    // Операторы инкремента и декремента
    iterator& operator++() {
      increment(Order());
      return *this;
    }

    iterator& operator--() {
      decrement(Order());
      return *this;
    }

    reference operator*() const {
      return node->value;
    }

    pointer operator->() const {
      return &(node->value);
    }

    bool operator==(const iterator& other) const {
      return node == other.node;
    }

    bool operator!=(const iterator& other) const {
      return node != other.node;
    }

   private:
    Node* node;

    void increment(InOrder) {
      if (node == nullptr) {
        return;
      }
      if (node->right != nullptr) {
        node = node->right;
        while (node->left != nullptr) {
          node = node->left;
        }
      } else {
        while (node->parent != nullptr && node == node->parent->right) {
          node = node->parent;
        }
        node = node->parent;
      }
    }

    void increment(PreOrder) {
      if (node == nullptr) {
        return;
      }
      if (node->left != nullptr) {
        node = node->left;
      } else if (node->right != nullptr) {
        node = node->right;
      } else {
        while (node->parent != nullptr && (node == node->parent->right || node->parent->right == nullptr)) {
          node = node->parent;
        }
        if (node->parent != nullptr) {
          node = node->parent->right;
        } else {
          node = nullptr;
        }
      }
    }

    void increment(PostOrder) {
      if (node == nullptr) {
        return;
      }
      if (node->parent == nullptr || node == node->parent->left && node->parent->right != nullptr) {
        node = node->parent->right;
        while (node->left != nullptr || node->right != nullptr) {
          while (node->left != nullptr) {
            node = node->left;
          }
          if (node->right != nullptr) {
            node = node->right;
          }
        }
      } else {
        node = node->parent;
      }
    }

    void decrement(InOrder) {
      if (node == nullptr) {
        return;
      }
      if (node->left != nullptr) {
        node = node->left;
        while (node->right != nullptr) {
          node = node->right;
        }
      } else {
        while (node->parent != nullptr && node == node->parent->left) {
          node = node->parent;
        }
        node = node->parent;
      }
    }

    void decrement(PreOrder) {
      if (node == nullptr || node->parent == nullptr) {
        // Если узел nullptr или корень дерева, декремент невозможен.
        return;
      }

      if (node == node->parent->right) {
        // Если текущий узел является правым потомком
        if (node->parent->left != nullptr) {
          // Если у родителя есть левый потомок, переходим к самому правому узлу в этом поддереве
          node = node->parent->left;
          while (node->right != nullptr || node->left != nullptr) {
            while (node->right != nullptr) {
              node = node->right;
            }
            if (node->left != nullptr) {
              node = node->left;
            }
          }
        } else {
          // Если у родителя нет левого потомка, предыдущим узлом является родитель
          node = node->parent;
        }
      } else if (node == node->parent->left) {
        // Если текущий узел является левым потомком, предыдущим узлом является родитель
        node = node->parent;
      }
    }

    void decrement(PostOrder) {
      if (node == nullptr) {
        return;
      }
      if (node->parent == nullptr) {
        node = nullptr;
      } else if (node == node->parent->right || node->parent->left == nullptr) {
        node = node->parent;
      } else if (node->parent->right != nullptr) {
        node = node->parent->right;
        while (node->left != nullptr || node->right != nullptr) {
          while (node->right != nullptr) {
            node = node->right;
          }
          if (node->left != nullptr) {
            node = node->left;
          }
        }
      } else {
        node = node->parent->left;
        while (node->left != nullptr || node->right != nullptr) {
          while (node->right != nullptr) {
            node = node->right;
          }
          if (node->left != nullptr) {
            node = node->left;
          }
        }
      }
    }
  };

  template<typename Order>
  class const_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;  // Константный указатель
    using reference = const T&;  // Константная ссылка
   private:
    // Внутренние данные, такие как указатель на текущий узел дерева
  };

  using iterator_in_order = iterator<InOrder>;
  using iterator_pre_order = iterator<PreOrder>;
  using iterator_post_order = iterator<PostOrder>;
  using const_iterator_in_order = const_iterator<InOrder>;
  using const_iterator_pre_order = const_iterator<PreOrder>;
  using const_iterator_post_order = const_iterator<PostOrder>;
  using reverse_iterator_in_order = std::reverse_iterator<iterator<InOrder>>;
  using const_reverse_iterator_in_order = std::reverse_iterator<const_iterator<InOrder>>;

  // Конструкторы и деструктор
  void clear(Node* node) noexcept {
    if (node) {
      clear(node->left);
      clear(node->right);
      node_allocator_.destroy(node);
      node_allocator_.deallocate(node, 1);
    }
  }
  void clear() noexcept {
    clear(root);
    root = nullptr;
  }
  Node* copy(Node* node, Node* parent = nullptr) {
    if (!node) {
      return nullptr;
    }
    Node* new_node = node_allocator_.allocate(1);
    node_allocator_.construct(new_node, node->value, parent);
    new_node->left = copy(node->left, new_node);
    new_node->right = copy(node->right, new_node);
    return new_node;
  }
  BinarySearchTree() noexcept: node_allocator_(allocator_type()), root(nullptr) {}
  BinarySearchTree(const BinarySearchTree& other) {
    root = copy(other.root);
  }
  ~BinarySearchTree() {
    clear(); // Вызов функции, очищающей дерево
  }

  // Оператор присваивания
  // Функция обмена
  void swap(BinarySearchTree& other) noexcept {
    using std::swap;
    swap(root, other.root);
    swap(node_allocator_, other.node_allocator_);
    // Обмен других членов класса, если они есть
  }
  BinarySearchTree& operator=(const BinarySearchTree& other) {
    swap(other);
    return *this;
  }

  // Методы для работы с узлами
  Node* allocateNode(const value_type& value) {
    Node* node = node_allocator_.allocate(1); // Используем NodeAllocator для выделения памяти для одного узла
    try {
      node_allocator_.construct(node, value); // Используем NodeAllocator для конструирования узла
    } catch (...) {
      node_allocator_.deallocate(node, 1); // В случае исключения освобождаем выделенную память
      throw; // Перебрасываем исключение
    }
    return node;
  }

  void deallocateNode(Node* node) {
    if (node != nullptr) {
      node_allocator_.destroy(node); // Используем NodeAllocator для уничтожения узла
      node_allocator_.deallocate(node, 1); // Используем NodeAllocator для освобождения памяти
    }
  }

  // Методы для работы с элементами
  void insert(const value_type& value) {
    Node* newNode = allocateNode(value); // Вызов функции для создания нового узла
    if (root == nullptr) {
      root = newNode; // Если дерево пустое, новый узел становится корнем
    } else {
      Node* current = root;
      Node* parent = nullptr;
      while (current != nullptr) {
        parent = current; // Сохраняем текущий узел как родителя для будущей вставки
        if (value < current->value) {
          current = current->left; // Переходим к левому поддереву
        } else {
          current = current->right; // Переходим к правому поддереву
        }
      }
      // Определяем, к какому потомку родительского узла присоединить новый узел
      if (value < parent->value) {
        parent->left = newNode;
      } else {
        parent->right = newNode;
      }
      newNode->parent = parent; // Устанавливаем связь родителя с новым узлом
    }
  }

  iterator<InOrder> find(const value_type& value) {
    Node* current = root; // Начинаем поиск с корня дерева
    while (current != nullptr) {
      if (value == current->value) {
        return iterator<InOrder>(current); // Найденный узел возвращается как итератор
      } else if (value < current->value) {
        current = current->left; // Переход к левому поддереву
      } else {
        current = current->right; // Переход к правому поддереву
      }
    }
    return iterator<InOrder>(nullptr); // Возвращаем итератор на nullptr, если значение не найдено
  }

  bool exist(const value_type& value) {
    Node* current = root; // Начинаем поиск с корня дерева
    while (current != nullptr) {
      if (value == current->value) {
        return true; // Если значение найдено, возвращаем true
      } else if (value < current->value) {
        current = current->left; // Переход к левому поддереву, если значение меньше текущего
      } else {
        current = current->right; // Переход к правому поддереву, если значение больше текущего
      }
    }
    return false; // Если значение не найдено в дереве, возвращаем false
  }

  iterator<InOrder> findMin() {
    if (root == nullptr) {
      return iterator<InOrder>(nullptr); // Если дерево пустое, возвращаем итератор, указывающий на nullptr
    }

    Node* current = root;
    while (current->left != nullptr) { // Переходим к самому левому узлу
      current = current->left;
    }
    return iterator<InOrder>(current); // Возвращаем итератор, указывающий на минимальный элемент
  }

  iterator<InOrder> findMax() {
    if (root == nullptr) {
      return iterator<InOrder>(nullptr); // Если дерево пустое, возвращаем итератор, указывающий на nullptr
    }

    Node* current = root;
    while (current->right != nullptr) { // Переходим к самому правому узлу
      current = current->right;
    }
    return iterator<InOrder>(current); // Возвращаем итератор, указывающий на максимальный элемент
  }

  // Дополнительные методы для работы с элементами
  size_type erase(const value_type& value) {
    Node* current = root;
    Node* parent = nullptr;
    size_type erased = 0;

    // Поиск узла с заданным значением
    while (current != nullptr) {
      if (value == current->value) {
        break;
      }
      parent = current;
      if (Compare()(value, current->value)) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    if (current == nullptr) {
      return 0; // Узел с таким значением не найден
    }

    // Удаляемый узел имеет двух детей
    if (current->left != nullptr && current->right != nullptr) {
      // Находим преемника (наименьший узел в правом поддереве удаляемого узла)
      Node* successor = current->right;
      while (successor->left != nullptr) {
        successor = successor->left;
      }
      // Копируем значение преемника в удаляемый узел
      current->value = successor->value;
      // Удаляем узел-преемник
      current = successor;
      // Продолжаем с одним из предыдущих случаев, т.к. у преемника не может быть двух детей
    }

    // Удаляемый узел имеет максимум одного ребенка
    Node* replacement = (current->left != nullptr) ? current->left : current->right;

    if (replacement != nullptr) {
      // Удаляемый узел не является корнем
      if (parent != nullptr) {
        if (parent->left == current) {
          parent->left = replacement;
        } else {
          parent->right = replacement;
        }
      } else {
        root = replacement; // Удаляемый узел является корнем
      }
      replacement->parent = parent;
      deallocateNode(current);
      erased = 1;
    } else if (parent == nullptr) {
      // Удаляемый узел - это корень, но у него нет детей
      deallocateNode(current);
      root = nullptr;
      erased = 1;
    } else {
      // Удаляемый узел не является корнем и не имеет детей
      if (parent->left == current) {
        parent->left = nullptr;
      } else {
        parent->right = nullptr;
      }
      deallocateNode(current);
      erased = 1;
    }
    return erased;
  }

  iterator<InOrder> extract(const_iterator<InOrder> position);
  void merge(BinarySearchTree& source);
  size_type count(const value_type& value) const;
  iterator<InOrder> lower_bound(const value_type& value);
  iterator<InOrder> upper_bound(const value_type& value);
  bool contains(const value_type& value) const;
  std::pair<iterator<InOrder>, iterator<InOrder>> equal_range(const value_type& value);

  // Методы контейнера
  iterator<InOrder> begin() noexcept;
  const_iterator<InOrder> cbegin() const noexcept;
  iterator<InOrder> end() noexcept;
  const_iterator<InOrder> cend() const noexcept;
  reverse_iterator_in_order rbegin() noexcept;
  const_reverse_iterator_in_order crbegin() const noexcept;
  reverse_iterator_in_order rend() noexcept;
  const_reverse_iterator_in_order crend() const noexcept;

  bool empty() const noexcept {
    return root == nullptr;
  }

  size_type countNodes(Node* node) const {
    if (node == nullptr) {
      return 0;
    } else {
      return 1 + countNodes(node->left) + countNodes(node->right);
    }
  }

  size_type size() const noexcept {
    return countNodes(root);
  }

  size_type max_size() const noexcept {
    return std::allocator_traits<allocator_type>::max_size(node_allocator_);
  }

  allocator_type get_allocator() const noexcept {
    return node_allocator_;
  }
};
