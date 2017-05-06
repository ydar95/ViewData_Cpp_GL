#ifndef _YDAR_HEAD_CIRCULAR_QUEUE_GL_H_
#define _YDAR_HEAD_CIRCULAR_QUEUE_GL_H_

#include<cstdint>
#include<memory>
template<class Ty>
class CircularQueue
{
public:
	CircularQueue(uint32_t In_left = 2, uint32_t In_right = 2) 
		:_left(In_left), _right(In_right), _size(In_left + In_right + 1)
	{
		_ptr= std::shared_ptr<Ty>(new Ty[_size], std::default_delete<Ty[]>());
		_index_crrent = 0;//
	}
	void moveLeft() {
		_index_crrent--;
		_index_updata = (((_index_crrent + 1 + _right)%_size)+ _size)%_size;
	}
	void moveRight() {
		_index_crrent++;
		_index_updata = (((_index_crrent - 1 - _left) % _size) + _size) % _size;
	}
	Ty&  getPrepareUpdataObject() {
		return _ptr.get()[_index_updata];
	}
	~CircularQueue() {
		NULL;
	}

	Ty& operator [](int32_t index) {
		//auto tmp = (_index_crrent + index) % _size;
		int32_t t_idx = (((_index_crrent+ index)%_size)+_size)%_size;
		return _ptr.get()[t_idx];
	}
private:
	int32_t _left;
	int32_t _right;
	int32_t _size;
	int32_t _index_updata;
	int32_t _index_crrent;
	std::shared_ptr<Ty> _ptr;
};
#endif
