#include <cstdio>

template <typename T>
class smart_ptr_base
{
public:
    smart_ptr_base(T* ptr)
    {
        ref = ptr;
        ref_count = new unsigned;
        *ref_count = 1;
        is_checked_on_null = true;
    }

    smart_ptr_base(const smart_ptr_base& sptr)
    {
        ref = sptr.ref;
        ref_count = sptr.ref_count;
        ++(*ref_count);
        is_checked_on_null = false;
    }

    smart_ptr_base& operator=(const smart_ptr_base& sptr)
    {
        if (this != &sptr)
        {
            //decrease the number of references of the old pointer and increase the number of references of the new one
            if (*ref_count > 0)
                remove();

            ref = sptr.ref;
            ref_count = sptr.ref_count;
            ++(*ref_count);
            is_checked_on_null = false;
        }
        return *this;
    }

    unsigned use_count() const
    {
        return *ref_count;
    }

    explicit operator bool()
    {
        is_checked_on_null = true;
        return ref != nullptr;
    }

    T* get() const {
      check_on_null();
      return ref;
    }

    T& operator*() const
    {
      return *get();
    }

    T* operator->() const
    {
      return get();
    }

protected:
    virtual void remove() = 0;

    void check_on_null() const
    {
      if (!is_checked_on_null)
          std::fprintf(stderr, "Warning: pointer is used without checking on null\n");
    }

    T* ref;
    unsigned* ref_count;
    bool is_checked_on_null;
};

template <typename T>
class smart_ptr : public smart_ptr_base<T>
{
public:
    using smart_ptr_base<T>::smart_ptr_base; // inherit constructors from the base class
    ~smart_ptr()
    {
        remove();
    }

private:
    void remove() override
    {
        if (--(*this->ref_count) == 0)
        {
            delete this->ref;
            delete this->ref_count;
            this->ref = nullptr;
            this->ref_count = nullptr;
        }
    }
};

template <typename T>
class smart_ptr<T[]> : public smart_ptr_base<T>
{
public:
    using smart_ptr_base<T>::smart_ptr_base; // inherit constructors from the base class
    ~smart_ptr()
    {
        remove();
    }

    T& operator[](size_t idx) const
    {
      return *(this->get() + idx);
    }

private:
    void remove() override
    {
        if (--(*this->ref_count) == 0)
        {
            delete[] this->ref;
            delete this->ref_count;
            this->ref = nullptr;
            this->ref_count = nullptr;
        }
    }
};
