/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	main.cpp
 * @brief	MultiQueueの動作確認コマンド。
 * @author	Yasutaka SHINDOH / 新堂 安孝
 */

#include <cstddef>
#include <cstdio>
#include "multi_queue.hpp"

/**
 * 動作確認用コマンド
 */
int main()
{
	char data[] = "ccccddcdcdccdd";
	ys::MultiQueue<char, 2>* queue = new ys::MultiQueue<char, 2>();

	for (size_t i(0); data[i]; ++i) {
		queue->enqueue(data[i] == 'c' ? 0 : 1, data[i]);
	}

	if (!queue->empty(1)) {
		std::printf("[%lu] %c\n", queue->size(1), queue->front(1));
		queue->dequeue(1);
	}

	while (!queue->empty(0)) {
		std::printf("[%lu] %c\n", queue->size(0), queue->front(0));
		queue->dequeue(0);
	}

	while (!queue->empty()) {
		std::printf("[%lu] %c\n", queue->size(), queue->front());
		queue->dequeue();
	}

	delete queue;

	return 0;
}
