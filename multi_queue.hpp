/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	multi_queue.hpp
 * @brief	複数のqueueを1つのqueueとして見ことも個別のqueueとして見ることもできる拡張queue
 * @author	Yasutaka SHINDOH / 新堂 安孝
 */

#ifndef	__MULTI_QUEUE_HPP__
#define	__MULTI_QUEUE_HPP__ "multi_queue.hpp"

#include <cstddef>
#include <cstring>
#include <cassert>
#include <deque>

namespace ys
{
	/**
	 * @class	複数のqueueを1つのqueueとして見ことも個別のqueueとして見ることもできる拡張queue
	 * @note	テンプレートの型 @a TYPE はqueueのキーの型。
	 * @note	テンプレートの整数 @a SIZE は個別のqueueの総数。
	 */
	template<typename TYPE, size_t SIZE>
	class MultiQueue
	{
	private:

		std::deque<size_t> index_queue_;	///< 全体を管理するqueue
		std::deque<TYPE> data_queue_[SIZE];	///< データを持つ個別queue
		size_t remain_[SIZE];				///< @a index_queue_ と @a data_queue_ の要素数差
		size_t length_;

		/**
		 * @a index_queue_ の調整
		 */
		void
		adjust_deque()
			{
				size_t i;

				while (!index_queue_.empty()) {
					i = index_queue_.front();
					if (!remain_[i]) break;
					index_queue_.pop_front();
					--remain_[i];
				}
			}

	public:

		/**
		 * コンストラクタ
		 */
		MultiQueue()
			: length_(0)
			{
				std::memset((void*)remain_, 0, sizeof(remain_));
			}

		/**
		 * デストラクタ
		 */
		virtual
		~MultiQueue()
			{
				;
			}

		/**
		 * 要素数を取得
		 */
		size_t
		size() const
			{
				return length_;
			}

		/**
		 * 個別のdequeの要素数を取得
		 * @param[in]	i	個別queueのインデックス
		 */
		size_t
		size(size_t i) const
			{
				assert(i < SIZE);

				return data_queue_[i].size();
			}

		/**
		 * 空かどうか確認
		 * @return	true: 空, false: 空でない
		 * @note	最悪計算量はO(1)。
		 */
		bool
		empty() const
			{
				return index_queue_.empty();
			}

		/**
		 * 個別のdequeが空かどうか確認
		 * @param[in]	i	個別queueのインデックス
		 * @return	true: 空, false: 空でない
		 * @note	最悪計算量はO(1)。
		 */
		bool
		empty(size_t i) const
			{
				assert(i < SIZE);

				return data_queue_[i].empty();
			}

		/**
		 * 先頭要素を取得
		 * @return	先頭要素
		 * @note	事前にqueueが空でないことを確認すること。
		 * @note	最悪計算量はO(1)。
		 */
		TYPE
		front() const
			{
				assert(!index_queue_.empty());

				size_t i = index_queue_.front();
				return data_queue_[i].front();
			}

		/**
		 * 個別queueの先頭要素を取得
		 * @param[in]	i	個別queueのインデックス
		 * @return	個別queueの先頭要素
		 * @note	事前にqueueが空でないことを確認すること。
		 * @note	最悪計算量はO(1)。
		 */
		TYPE
		front(size_t i) const
			{
				assert(i < SIZE);
				assert(!data_queue_[i].empty());

				return data_queue_[i].front();
			}

		/**
		 * 個別queueに要素を追加
		 * @param[in]	i	個別queueのインデックス
		 * @param[in]	data	追加するデータ
		 * @note	最悪計算量はO(1)。
		 */
		void
		enqueue(size_t i,
				const TYPE& data)
			{
				assert(i < SIZE);

				index_queue_.push_back(i);
				data_queue_[i].push_back(data);
				++length_;
			}

		/**
		 * 要素を削除
		 * @note	事前にqueueが空でないことを確認すること。
		 */
		void
		dequeue()
			{
				assert(!index_queue_.empty());

				size_t i = index_queue_.front();
				index_queue_.pop_front();
				data_queue_[i].pop_front();
				adjust_deque();
				--length_;
			}

		/**
		 * 個別queueの要素を削除
		 * @param[in]	i	個別queueのインデックス
		 * @note	事前にqueueが空でないことを確認すること。
		 */
		void
		dequeue(size_t i)
			{
				assert(i < SIZE);
				assert(!data_queue_[i].empty());

				data_queue_[i].pop_front();
				if (i == index_queue_.front()) {
					index_queue_.pop_front();
					adjust_deque();
				}
				else {
					++remain_[i];
				}
				--length_;
			}
	};
};

#endif	// __MULTI_QUEUE_HPP__
