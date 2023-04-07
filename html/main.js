
// const field = document.querySelector('.field')
const socket = new WebSocket('ws://localhost:56765/')
let highscore = 0

const field = document.createElement('div'),
	score = document.createElement('div')
scale = 20,
	fieldWidth = 600,
	fieldHeight = 600,
	snake = new Snake(scale),
	apple = new Apple(scale)

let xd = scale,
	yd = 0,
	count = 0

field.classList.add('field')
document.body.appendChild(field)
field.style.width = `${fieldWidth}px`
field.style.height = `${fieldHeight}px`
score.style.position = `absolute`
score.style.top = `610px`
document.body.appendChild(score)


snake.draw()
apple.draw()

socket.addEventListener('message', async event => highscore = event.data)

document.addEventListener('keydown', key => {
	switch (key.key) {
		case 'ArrowUp':
			if (yd === scale) break;
			yd = -scale
			xd = 0
			break;
		case 'ArrowDown':
			if (yd === -scale) break;
			yd = scale
			xd = 0
			break;
		case 'ArrowLeft':
			if (xd === scale) break;
			xd = -scale
			yd = 0
			break;
		case 'ArrowRight':
			if (xd === -scale) break;
			xd = scale
			yd = 0
			break;
		default:
			break;
	}
})

setInterval(() => {
	snake.move(xd, yd)
	apple.eaten(snake)
	counter()
}, 200)

function counter() {
	score.innerHTML = `
	<h1>Score: ${count}</h1>
	<h1>HighScore: ${highscore}</h1>`
}
