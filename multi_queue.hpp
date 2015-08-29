/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	multi_queue.hpp
 * @brief	複数のqueueを1つのqueueとして見ことも個別queueとして見ることもできる拡張queue
 * @author	Yasutaka SHINDOH / 新堂 安孝
 */

#ifndef	__MULTI_QUEUE_HPP__
#define	__MULTI_QUEUE_HPP__ "multi_queue.hpp"

#include <cstddef>
#include <cstring>
#include <climits>
#include <cassert>
#include <utility>
#include <deque>

namespace ys
{
	/**
	 * @class	複数のqueueを1つのqueueとして見ことも個別queueとして見ることもできる拡張queue
	 * @note	テンプレートの型 @a TYPE は個別queueに格納するデータの型。
	 * @note	テンプレートの整数 @a SIZE は個別queueの総数。
	 */
	template<typename TYPE, size_t SIZE>
	class MultiQueue
	{
	private:

		/**
		 * queueに確保する要素の型
		 * @note	第1要素はID。
		 * @note	第2要素は本来の格納対象のデータ。
		 */
		typedef	std::pair<size_t, TYPE>	Unit;

		std::deque<Unit> queues_[SIZE];	///< データを格納する個別queue
		size_t count_;					///< 格納した要素ののべ数 (型を超える数は扱えない)
		size_t length_;					///< queue全体が持つ要素数

		/**
		 * 最も古い先頭要素を持つ個別queueを探索
		 * @return	個別queueのインデックス
		 * @note	探索失敗時は SIZE が返却される。
		 * @note	計算量は O(1)。ただし SIZE を定数と仮定する。
		 */
		size_t
		find_oldest_front() const
			{
				size_t k(SIZE);
				size_t id, t;

				for (size_t i(0); i < SIZE; ++i) {
					if (queues_[i].empty()) continue;
					t = queues_[i].front().first;
					if (k < SIZE && id <= t) continue;
					id = t;
					k = i;
				}

				return k;
			}

	public:

		/**
		 * コンストラクタ
		 */
		MultiQueue()
			: count_(0), length_(0)
			{
				;
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
		 * @note	計算量は O(1)。
		 */
		size_t
		size() const
			{
				return length_;
			}

		/**
		 * 個別queueの要素数を取得
		 * @param[in]	i	個別queueのインデックス
		 * @note	計算量は O(1)。
		 */
		size_t
		size(size_t i) const
			{
				assert(i < SIZE);

				return queues_[i].size();
			}

		/**
		 * 空かどうか確認
		 * @return	true: 空, false: 空でない
		 * @note	計算量は O(1)。
		 */
		bool
		empty() const
			{
				return 0 == length_;
			}

		/**
		 * 個別queueが空かどうか確認
		 * @param[in]	i	個別queueのインデックス
		 * @return	true: 空, false: 空でない
		 * @note	計算量は O(1)。
		 */
		bool
		empty(size_t i) const
			{
				assert(i < SIZE);

				return queues_[i].empty();
			}

		/**
		 * 先頭要素を取得
		 * @return	先頭要素
		 * @note	事前に空でないことを確認すること。
		 * @note	計算量は O(1)。ただし SIZE を定数と仮定する。
		 */
		TYPE
		front() const
			{
				assert(0 < length_);

				size_t i = find_oldest_front();
				assert(i < SIZE);

				return queues_[i].front().second;
			}

		/**
		 * 個別queueの先頭要素を取得
		 * @param[in]	i	個別queueのインデックス
		 * @return	個別queueの先頭要素
		 * @note	事前に個別queueが空でないことを確認すること。
		 * @note	計算量は O(1)。
		 */
		TYPE
		front(size_t i) const
			{
				assert(i < SIZE);
				assert(!queues_[i].empty());

				return queues_[i].front().second;
			}

		/**
		 * 個別queueに要素を格納
		 * @param[in]	i	個別queueのインデックス
		 * @param[in]	data	追加するデータ
		 * @return	true: 格納成功, false: 格納失敗
		 * @note	計算量は O(1)。
		 */
		bool
		enqueue(size_t i,
				const TYPE& data)
			{
				assert(i < SIZE);

				assert(count_ < ULONG_MAX);
				if (ULONG_MAX <= count_) return false;

				try {
					queues_[i].push_back(std::pair<size_t, TYPE>(count_, data));
				}
				catch (...) {
					assert(false);
					return false;
				}

				++count_;
				++length_;
				return true;
			}

		/**
		 * 先頭要素を削除
		 * @note	queueが空の時は何もしない。
		 * @note	計算量は O(1)。ただし SIZE を定数と仮定する。
		 */
		void
		dequeue()
			{
				if (0 < length_) {
					size_t i = find_oldest_front();
					assert(i < SIZE);
					queues_[i].pop_front();
					--length_;
					if (length_ == 0) count_ = 0;	// カウンタをリセット
				}
			}

		/**
		 * 個別queueの先頭要素を削除
		 * @param[in]	i	個別queueのインデックス
		 * @note	該当queueが空の時は何もしない。
		 * @note	計算量は O(1)。
		 */
		void
		dequeue(size_t i)
			{
				assert(i < SIZE);

				if(0 < queues_[i].size()) {
					queues_[i].pop_front();
					--length_;
					if (length_ == 0) count_ = 0;	// カウンタをリセット
				}
			}
	};
};

#endif	// __MULTI_QUEUE_HPP__
