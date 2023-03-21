import { check } from 'k6'
import http from 'k6/http'

export let options = {
    stages: [
        { duration: '10s', target: 1 },
        { duration: '10s', target: 1 },
        { duration: '10s', target: 10 },
        { duration: '10s', target: 10 },
        { duration: '10s', target: 100 },
        { duration: '10s', target: 100 },
        { duration: '10s', target: 1000 },
        { duration: '10s', target: 1000 },
        { duration: '10s', target: 100 },
        { duration: '10s', target: 100 },
        { duration: '10s', target: 10 },
        { duration: '10s', target: 10 },
        { duration: '10s', target: 1 },
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
