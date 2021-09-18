/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: ULru.h
@Time: 2021/9/18 11:55 下午
@Desc: 
***************************/

#ifndef CGRAPH_ULRU_H
#define CGRAPH_ULRU_H

#include <list>
#include <unordered_map>

#include "../UtilsObject.h"
#include "ULruNode.h"

const static unsigned int DEFAULT_LRU_CACHE_CAPACITY = 10;

template<typename K, typename V>
class ULru : public UtilsObject {
public:
    explicit ULru(unsigned int capacity) {
        this->capacity_ = capacity;
    }

    explicit ULru() = default;

    ~ULru() override {
        clear();
    }

    /**
     * 信息插入Lru
     * @param key
     * @param value
     */
    void put(const K& key, const V& value) {
        auto cur = cache_.find(key);
        if (cur != cache_.end()) {
            /* 如果在cache中找到，则删除（稍后更新至最前） */
            nodes_.erase(cache_[key]);
            cache_.erase(key);
            cur_size_--;
        } else if (cur_size_ >= capacity_) {
            /* 如果超限，则删除最后一个节点信息 */
            auto back = nodes_.back();
            cache_.erase(back.key_);
            nodes_.pop_back();
            cur_size_--;
        }

        nodes_.push_front(ULruNode<K, V>(key, value));
        cache_[key] = nodes_.begin();
        cur_size_++;
    }

    /**
     * 从Lru中获取节点信息
     * @param key
     * @return
     */
    V get(const K& key) {
        V result;
        auto cur = cache_.find(key);
        if (cur != cache_.end()) {
            result = cur->second->value_;
            put(key, result);
        }

        return result;
    }

    /**
     * 设置capacity信息
     * @param capacity
     */
    void setCapacity(unsigned int capacity) {
        this->capacity_ = capacity;
    }

    /**
     * 获取capacity信息
     * @return
     */
    unsigned int getCapacity() {
        return this->capacity_;
    }

    /**
     * 清空Lru中的信息
     */
    void clear() {
        nodes_.clear();
        cache_.clear();
        cur_size_ = 0;
    }


private:
    unsigned int cur_size_ { 0 };                                                        // 当前尺寸
    unsigned int capacity_ { DEFAULT_LRU_CACHE_CAPACITY };                               // 最大容量
    std::unordered_map<K, typename std::list<ULruNode<K, V>>::iterator> cache_;          // 缓存信息
    std::list<ULruNode<K, V>> nodes_;                                                    // 节点链表
};

#endif //CGRAPH_ULRU_H