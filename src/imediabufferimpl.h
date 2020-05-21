#ifndef IMEDIABUFFERIMPL_H
#define IMEDIABUFFERIMPL_H

#include <mediaobj.h>

class MediaBufferImpl : public IMediaBuffer
{
public:
    explicit MediaBufferImpl(DWORD maxLength)
        : _data(new BYTE[maxLength]),
          _length(0),
          _maxLength(maxLength),
          _refCount(0)
    {}
    // IMediaBuffer methods.
    HRESULT GetBufferAndLength(BYTE** ppBuffer, DWORD* pcbLength) __attribute__(stdcall_)
    {
        if (!ppBuffer || !pcbLength)
        {
            return E_POINTER;
        }
        *ppBuffer = _data;
        *pcbLength = _length;
        return S_OK;
    }
    STDMETHOD(GetMaxLength(DWORD* pcbMaxLength))
    {
        if (!pcbMaxLength)
        {
            return E_POINTER;
        }
        *pcbMaxLength = _maxLength;
        return S_OK;
    }
    STDMETHOD(SetLength(DWORD cbLength))
    {
        if (cbLength > _maxLength)
        {
            return E_INVALIDARG;
        }
        _length = cbLength;
        return S_OK;
    }
    // IUnknown methods.
    STDMETHOD_(ULONG, AddRef())
    {
        return InterlockedIncrement(&_refCount);
    }
    STDMETHOD(QueryInterface(REFIID riid, void** ppv))
    {
        if (!ppv)
        {
            return E_POINTER;
        }
        else if (riid != IID_IMediaBuffer && riid != IID_IUnknown)
        {
            return E_NOINTERFACE;
        }
        *ppv = static_cast<IMediaBuffer*>(this);
        AddRef();
        return S_OK;
    }
    STDMETHOD_(ULONG, Release())
    {
        LONG refCount = InterlockedDecrement(&_refCount);
        if (refCount == 0)
        {
            delete this;
        }
        return refCount;
    }
private:
    ~MediaBufferImpl()
    {
        delete [] _data;
    }
    BYTE* _data;
    DWORD _length;
    const DWORD _maxLength;
    LONG _refCount;
};

#endif // IMEDIABUFFERIMPL_H
