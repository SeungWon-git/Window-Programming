# 🦍 Donkey Kong (동키콩 모작)

## 🎮 개요
 - 게임 장르: 플랫포머 게임
 - 플랫폼: PC
 - 플레이 인원: 1인
 - 1인 개발
 - **구현 내용**:
   > + **WinAPI를 활용**하여 2D 게임 개발
   > + 스프라이트 애니메이션, 인트로, 엔딩씬 애니메이션 (→ **더블 버퍼링** 적용)
   > + 총 5개의 스테이지(+ 스테이지 별 새로운 장애물들), 망치 아이템
   > + 사운드(+ 소리 조절 기능-배경음/효과음 개별적으로), 일시정지 기능, 다시시작 기능
 - 개발에 사용된 기술 스택:
   + WinAPI
   + C++
 - 시연 영상: [▶️ 유투브 영상 보기](https://www.youtube.com/)
<img width="700" height="727" alt="image" src="https://github.com/user-attachments/assets/cd3ccedf-d860-4ea3-b62f-f4214c8e241c" />


---

## 📝 구현 내용 상세 설명
   
### 🧱 계단 오르기 알고리즘
 - **부드러운 계단(플랫폼) 상호작용**을 위해 신경을 많이 씀!
 - 
 * 구현 코드: [플랫폼과 상호작용처리](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L1174)

### 🎨 더블 버퍼링을 이용한 부드러운 스프라이트 애니메이션
 - 
 * 구현 코드: [더블 버퍼링 설정 및 화면 그리기 절차](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L696)
   
### 🎞️ 인트로, 엔딩씬 애니메이션 (+ 맵 저장 방식)
 - 
 * 구현 코드: []()
   
### 🛢️ 장애물 설명
 - 중력 적용
 - 원통의 사다리/벽 반응
 - 망치 아이템 충돌체크
 * 구현 코드: []()
  
### 🔊 fmod 사운드 라이브러리를 이용하여 사운드 구현
 - 다양한 배경음, 마리오의 걷기-점프-피격-사다리 타기-망치 아이템 사용, 장애물 파괴 등등 **다양한 곳에서 사운드 이펙트가 적용**함
 - 배경음과 효과음 사운드는 각각 키보드 입력을 통해 **따로 소리 크기 조절 & 음소거 ON/OFF가 가능**하다.
 * 구현 코드: [배경/효과음 크기 조절](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L627), [배경/효과음 음소거 ON/OFF](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L250), [배경음 전환](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L2051)

  
---

## 📎 기타
- 소스 코드: 👉 [소스 코드](https://github.com/SeungWon-git/Window-Programming/tree/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C)
- 조작키 설명: ⌨️ [조작키](https://github.com/SeungWon-git/Window-Programming/blob/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/%EB%8F%99%ED%82%A4%EC%BD%A9%20-%20%EB%B0%B0%ED%8F%AC/%EC%A1%B0%EC%9E%91%ED%82%A4%20%EC%84%A4%EB%AA%85.txt)
