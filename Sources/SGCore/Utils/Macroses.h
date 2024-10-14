#pragma once

/*
* MisterElect (MisterChoose)
* 02.10.2024
*/

#define move_operator(T) T& operator=(T&& other) noexcept
#define move_constructor(T) explicit T(T&& other) noexcept
#define copy_operator(T) T& operator=(const T& other) noexcept
#define copy_constructor(T) explicit T(const T& other) noexcept