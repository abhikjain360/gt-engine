#include <algorithm>
#include <memory>
#include <type_traits>

// used internally, inside vertex to point to other vertices
struct joiner {
    float weight;
    unsigned int dest;
};

// Note: using template means need to be known at compile time
// change the constructor, removing template if known at run time
class vertex {
public:
	/* Construtors */
    vertex(unsigned int capacity = 2, unsigned int weight = 1)
        : _Deg(0),
          _W(weight),
          _Cap(capacity),
          _E(std::make_unique<joiner[]>(capacity)) {}

    vertex(std::unique_ptr<joiner[]> joiner_list, unsigned int size,
           unsigned int weight = 1)
        : _Deg(size), _W(weight), _Cap(size), _E(std::move(joiner_list)) {}

	/* Setters */
    void set_weight(unsigned int weight) { this->_W = weight; }

	/* Getters */
	unsigned int weight() { return _W; }
	unsigned int capacity() { return _Cap; }
	unsigned int degree() { return _Deg; }

	/* Resize */
    void resize(unsigned int size) {
        auto temp = std::make_unique<joiner[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<joiner[]>) {
            std::move(_E.get(), _E.get() + _Cap, temp.get());
        } else {
            std::copy(_E.get(), _E.get() + _Cap, temp.get());
        }

        _E   = std::move(temp);
        _Cap = size;
    }

	/* Add edges */
    void join(unsigned int dest, float weight) {
        if (_Deg == _Cap) {
            int lt = 2;
            while (lt <= _Cap) lt <<= 1;
            this->resize(lt);
        }

        _E[++_Deg] = {weight, dest};
    }

private:
    unsigned int _Deg, _W, _Cap;
    std::unique_ptr<joiner[]> _E;
};

class graph {};
