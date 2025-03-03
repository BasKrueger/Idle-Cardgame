
template <typename T>
class Test
{
	public:
		T test(T t);
};

template<typename T>
inline T Test<T>::test(T t)
{
	return t;
}
