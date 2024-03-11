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

  template<typename Order>
  class const_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = const T; // Используем const T вместо T
    using difference_type = std::ptrdiff_t;
    using pointer = const T*; // Константный указатель
    using reference = const T&; // Константная ссылка

    // Конструктор
    const_iterator(const Node* node = nullptr, const Node* root = nullptr) : node(node), root(root) {}

    // Операторы инкремента и декремента
    const_iterator& operator++() {
      increment(Order());
      return *this;
    }

    const_iterator& operator--() {
      decrement(Order());
      return *this;
    }

    reference operator*() const {
      return node->value;
    }

    pointer operator->() const {
      return &(node->value);
    }

    bool operator==(const const_iterator& other) const {
      return node == other.node;
    }

    bool operator!=(const const_iterator& other) const {
      return node != other.node;
    }

    const Node* get_node() const { return node; }

   private:
    const Node* node;
    const Node* root; // Добавляем указатель на корень дерева


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
      // Если текущий узел - левый потомок и у родителя есть правый потомок
      if (node->parent != nullptr && node == node->parent->left && node->parent->right != nullptr) {
        node = node->parent->right; // Переход к правому потомку родителя
        // Ищем самый левый узел в поддереве правого потомка
        while (node->left != nullptr || node->right != nullptr) {
          while (node->left != nullptr) {
            node = node->left;
          }
          // Проверяем, не равен ли правый потомок nullptr перед переходом
          if (node->right != nullptr) {
            node = node->right;
          } else {
            break; // Выходим из цикла, если правый потомок равен nullptr
          }
        }
      } else {
        // Случай, когда текущий узел - правый потомок или у родителя нет правого потомка
        // Возвращаемся к родителю, но сначала проверяем, не равен ли он nullptr
        if (node->parent != nullptr) {
          node = node->parent;
        } else {
          node = nullptr; // Устанавливаем node в nullptr, если достигнут корень
        }
      }
    }

    void decrement(InOrder) {
      if (node == nullptr) {
        // Находим максимальный элемент в дереве, если текущий узел равен nullptr
        node = root;
        if (node == nullptr) return; // Пустое дерево
        while (node->right != nullptr) {
          node = node->right;
        }
      } else if (node->left != nullptr) {
        node = node->left;
        while (node->right != nullptr) {
          node = node->right;
        }
      } else {
        const Node* tmp = node;
        node = node->parent;
        while (node != nullptr && tmp == node->left) {
          tmp = node;
          node = node->parent;
        }
      }
    }

    void decrement(PreOrder) {
      if (node == nullptr) {
        // Если узел nullptr, декремент невозможен.
        return;
      }

      if (node->parent == nullptr) {
        // Если текущий узел - корень дерева, переходим к состоянию, указывающему на конец итерации
        node = nullptr;
        return;
      }

      if (node == node->parent->right && node->parent->left != nullptr) {
        // Если текущий узел является правым потомком и у родителя есть левый потомок
        node = node->parent->left;
        while (node->right != nullptr || node->left != nullptr) {
          // Переходим к самому правому узлу в поддереве левого потомка
          while (node->right != nullptr) {
            node = node->right;
          }
          if (node->left != nullptr) {
            node = node->left;
          }
        }
      } else {
        // В остальных случаях предыдущим узлом является родитель
        node = node->parent;
      }
    }

    void decrement(PostOrder) {
      if (node == nullptr) {
        node = root; // Начинаем с корня, если текущий узел не указан
      } else if (node->right != nullptr) {
        node = node->right; // Переходим к правому потомку, если он существует
      } else if (node->left != nullptr) {
        node = node->left; // В противном случае переходим к левому потомку, если он существует
      } else {
        // Если узел является листом, поднимаемся вверх по дереву
        Node* parent = node->parent;
        while (parent && (parent->left == nullptr || node == parent->left)) {
          node =
              parent; // Поднимаемся, пока текущий узел является левым потомком или пока родительский узел не имеет левого потомка
          parent = node->parent;
        }
        if (parent) {
          node = parent->left; // Переходим к левому потомку родительского узла, если таковой имеется
        } else {
          node = nullptr; // Иначе мы достигли корня, завершаем обход
        }
      }
    }
  };

  // Конструкторы и деструктор + методы для них
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
    clear();
  }

  // Функция обмена
  void swap(BinarySearchTree& other) noexcept {
    using std::swap;
    swap(root, other.root);
    swap(node_allocator_, other.node_allocator_);
  }

  BinarySearchTree& operator=(const BinarySearchTree& other) {
    swap(other);
    return *this;
  }

  // Методы для работы с узлами
  Node* allocateNode(const value_type& value) {
    Node* node = node_allocator_.allocate(1);
    try {
      node_allocator_.construct(node, value);
    } catch (...) {
      node_allocator_.deallocate(node, 1);
      throw;
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

  const_iterator<InOrder> find(const value_type& value) {
    Node* current = root; // Начинаем поиск с корня дерева
    while (current != nullptr) {
      if (value == current->value) {
        return const_iterator<InOrder>(current); // Найденный узел возвращается как итератор
      } else if (value < current->value) {
        current = current->left; // Переход к левому поддереву
      } else {
        current = current->right; // Переход к правому поддереву
      }
    }

    return const_iterator<InOrder>(nullptr); // Возвращаем итератор на nullptr, если значение не найдено
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

  const_iterator<InOrder> findMin() {
    if (root == nullptr) {

      return const_iterator<InOrder>(nullptr); // Если дерево пустое, возвращаем итератор, указывающий на nullptr
    }

    Node* current = root;
    while (current->left != nullptr) { // Переходим к самому левому узлу
      current = current->left;
    }

    return const_iterator<InOrder>(current); // Возвращаем итератор, указывающий на минимальный элемент
  }

  const_iterator<InOrder> findMax() {
    if (root == nullptr) {

      return const_iterator<InOrder>(nullptr); // Если дерево пустое, возвращаем итератор, указывающий на nullptr
    }

    Node* current = root;
    while (current->right != nullptr) { // Переходим к самому правому узлу
      current = current->right;
    }

    return const_iterator<InOrder>(current); // Возвращаем итератор, указывающий на максимальный элемент
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

  const_iterator<InOrder> extract(const_iterator<InOrder> position) {
    if (position.get_node() == nullptr) {

      return const_iterator<InOrder>(nullptr);
    }

    Node* nodeToRemove = const_cast<Node*>(position.get_node());
    Node* successor = nullptr;

    // Узел имеет двух потомков
    if (nodeToRemove->left != nullptr && nodeToRemove->right != nullptr) {
      successor = nodeToRemove->right;
      while (successor->left != nullptr) { // Находим преемника
        successor = successor->left;
      }
      nodeToRemove->value = successor->value; // Копируем значение преемника в удаляемый узел
      nodeToRemove = successor; // Теперь удаляем узел-преемник
    }

    // Узел имеет не более одного потомка (случаи 1 и 2)
    successor = (nodeToRemove->left != nullptr) ? nodeToRemove->left : nodeToRemove->right;

    if (successor != nullptr) { // Узел имеет одного потомка
      if (nodeToRemove->parent != nullptr) {
        if (nodeToRemove == nodeToRemove->parent->left) {
          nodeToRemove->parent->left = successor;
        } else {
          nodeToRemove->parent->right = successor;
        }
      } else {
        root = successor; // Удаляемый узел является корнем
      }
      successor->parent = nodeToRemove->parent;
    } else if (nodeToRemove->parent == nullptr) { // Удаляемый узел является корнем и не имеет потомков
      root = nullptr;
    } else { // Удаляемый узел не имеет потомков
      if (nodeToRemove == nodeToRemove->parent->left) {
        nodeToRemove->parent->left = nullptr;
      } else {
        nodeToRemove->parent->right = nullptr;
      }
    }

    // Освобождаем память удаляемого узла
    deallocateNode(nodeToRemove);

    return const_iterator<InOrder>(successor);
  }

  void insertNodesFrom(Node* node) {
    if (node != nullptr) {
      // Вставляем значение текущего узла
      insert(node->value);

      // Рекурсивно вставляем левое поддерево
      insertNodesFrom(node->left);

      // Рекурсивно вставляем правое поддерево
      insertNodesFrom(node->right);
    }
  }

  void merge(BinarySearchTree& source) {
    // Рекурсивно вставляем каждый узел из source в текущее дерево
    insertNodesFrom(source.root);

    // Очищаем исходное дерево после слияния
    source.clear();
  }

  size_type count(const value_type& value) const {
    Node* current = root; // Начинаем поиск с корня дерева
    while (current != nullptr) {
      if (value == current->value) {
        return 1; // Элемент найден, возвращаем 1
      } else if (Compare()(value, current->value)) {
        current = current->left; // Переходим к левому поддереву
      } else {
        current = current->right; // Переходим к правому поддереву
      }
    }

    return 0; // Элемент не найден, возвращаем 0
  }

  bool contains(const value_type& value) const {
    Node* current = root; // Начинаем поиск с корня дерева
    while (current != nullptr) {
      if (value == current->value) {
        return true; // Значение найдено
      } else if (Compare()(value, current->value)) {
        current = current->left; // Переходим к левому поддереву
      } else {
        current = current->right; // Переходим к правому поддереву
      }
    }

    return false; // Значение не найдено в дереве
  }

  const_iterator<InOrder> lower_bound(const value_type& value) const {
    const Node* node = root;
    const Node* result = nullptr; // Изначально устанавливаем результат на nullptr

    while (node != nullptr) {
      if (node->value < value) {
        // Если значение узла меньше искомого, идем вправо
        node = node->right;
      } else {
        // Если значение узла больше или равно, запоминаем узел и идем влево
        result = node;
        node = node->left;
      }
    }
    // Возвращаем итератор на найденный узел или на end, если узел не найден
    return const_iterator<InOrder>(result, root);
  }

  const_iterator<InOrder> upper_bound(const value_type& value) const {
    const Node* node = root;
    const Node* result = nullptr; // Изначально устанавливаем результат на nullptr

    while (node != nullptr) {
      if (node->value <= value) {
        // Если значение узла меньше или равно искомому, идем вправо
        node = node->right;
      } else {
        // Если значение узла строго больше, запоминаем узел и идем влево
        result = node;
        node = node->left;
      }
    }
    // Возвращаем итератор на найденный узел или на end, если узел не найден
    return const_iterator<InOrder>(result, root);
  }

  std::pair<const_iterator<InOrder>, const_iterator<InOrder>> equal_range(const value_type& value) const {

    return std::make_pair(lower_bound(value), upper_bound(value));
  }

  // Методы контейнера
  template<typename Order>
  const_iterator<Order> begin() const {
    if constexpr (std::is_same_v<Order, InOrder>) {
      Node* n = root;
      while (n && n->left) {
        n = n->left; // Находим наименьший элемент в дереве
      }

      return const_iterator<Order>(n);
    } else if constexpr (std::is_same_v<Order, PreOrder>) {
      return const_iterator<Order>(root); // В PreOrder обход начинается с корня
    } else if constexpr (std::is_same_v<Order, PostOrder>) {
      Node* n = root;
      if (n) {
        while (n->left || n->right) {
          while (n->left) {
            n = n->left;
          }
          if (n->right) {
            n = n->right;
          }
        }
      }

      return const_iterator<Order>(n); // В PostOrder обход начинается с самого левого листа
    }
  }

  template<typename Order>
  const_iterator<Order> end() const {

    return const_iterator<Order>(nullptr, root);
  }

  template<typename Order>
  const_iterator<Order> cbegin() const {
    if constexpr (std::is_same_v<Order, InOrder>) {
      Node* n = root;
      while (n && n->left) {
        n = n->left; // Находим наименьший элемент в дереве
      }

      return const_iterator<Order>(n);
    } else if constexpr (std::is_same_v<Order, PreOrder>) {
      return const_iterator<Order>(root); // В PreOrder обход начинается с корня
    } else if constexpr (std::is_same_v<Order, PostOrder>) {
      Node* n = root;
      if (n) {
        while (n->left || n->right) {
          while (n->left) {
            n = n->left;
          }
          if (n->right) {
            n = n->right;
          }
        }
      }

      return const_iterator<Order>(n); // В PostOrder обход начинается с самого левого листа
    }
  }

  template<typename Order>
  const_iterator<Order> cend() const {

    return const_iterator<Order>(nullptr, root); // Все обходы заканчиваются на nullptr
  }

  template<typename Order>
  const_iterator<Order> rbegin() const {
    if constexpr (std::is_same_v<Order, InOrder>) {
      Node* n = root;
      while (n && n->right) { // Идем по правой ветке, чтобы найти максимальный элемент
        n = n->right;
      }

      return const_iterator<Order>(n);
    } else if constexpr (std::is_same_v<Order, PreOrder>) {
      Node* n = root;
      while (n) { // Идем по правой ветке до самого конца
        if (n->right) {
          n = n->right;
        } else if (n->left) {
          n = n->left;
        } else {
          break; // Дошли до листа
        }
      }

      return const_iterator<Order>(n);
    } else if constexpr (std::is_same_v<Order, PostOrder>) {
      return const_iterator<Order>(root); // В PostOrder rbegin начинается с корня
    }
  }

  template<typename Order>
  const_iterator<Order> rend() const {

    return const_iterator<Order>(nullptr); // Во всех случаях rend указывает на nullptr
  }

  template<typename Order>
  const_iterator<Order> crbegin() const {
    if constexpr (std::is_same_v<Order, InOrder>) {
      Node* n = root;
      while (n && n->right) { // Идем по правой ветке, чтобы найти максимальный элемент
        n = n->right;
      }

      return const_iterator<Order>(n);
    } else if constexpr (std::is_same_v<Order, PreOrder>) {
      Node* n = root;
      while (n) { // Идем по правой ветке до самого конца
        if (n->right) {
          n = n->right;
        } else if (n->left) {
          n = n->left;
        } else {
          break; // Дошли до листа
        }
      }

      return const_iterator<Order>(n);
    } else if constexpr (std::is_same_v<Order, PostOrder>) {
      return const_iterator<Order>(root); // В PostOrder rbegin начинается с корня
    }
  }

  template<typename Order>
  const_iterator<Order> crend() const {

    return const_iterator<Order>(nullptr); // Во всех случаях rend указывает на nullptr
  }

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
