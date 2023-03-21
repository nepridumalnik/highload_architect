import { check } from 'k6'
import http from 'k6/http'

const time = '5s'

export let options = {
    stages: [
        { duration: time, target: 1 },
        { duration: time, target: 1 },
        { duration: time, target: 10 },
        { duration: time, target: 10 },
        { duration: time, target: 100 },
        { duration: time, target: 100 },
        { duration: time, target: 1000 },
        { duration: time, target: 1000 },
        { duration: time, target: 100 },
        { duration: time, target: 100 },
        { duration: time, target: 10 },
        { duration: time, target: 10 },
        { duration: time, target: 1 },
    ]
}

export default function () {
    const url = 'http://localhost:80/user/search'
    const params = { headers: { 'Content-Type': 'application/json' } };
    const user = JSON.stringify({
        first_name: 'Иван%', second_name: 'Бород%'
    })

    const res = http.post(url, user, params)
    check(res, { 'status 200': (r) => r.status === 200, })
}
