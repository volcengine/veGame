import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { CloudGameView } from './components/cloudgame'

createRoot(document.getElementById('root')).render(
  <StrictMode>
    <CloudGameView />
  </StrictMode>,
)
