const yearAverage = 31557286.9565217 // Average Seconds on Every Year [1970-2038]
const yearRange = range( 1970, 2038 )
const monthRange = range( 1, 12 )
const timeZoneOffset = 9 * 60 * 60 // JST+0900
const restrainValue = false // If `true`, sets the anomaly normalized value (less than 0.0 or greater than 1.0) to 0.0 or 1.0


function main() {
  let averages = []
  for ( let month of monthRange ) {
    averages.push( calcAverage( month ) )
  }
  console.log( averages )
}


// The param `month` sould be in the range 1 to 12 ( Jan. to Dec. )
function calcAverage( month ) {
  let filteredCount = 0
  let results = [
    [], // 1st element: start of the month
    [], // 2nd element: end of the month
  ]

  for ( let year of yearRange ) {

    const lastDay = getLastDayOfMonth( year, month )

    const monthStart = Math.floor( new Date( year, month - 1, 1, 0, 0, 0, 0 ).getTime() / 1000 ) + timeZoneOffset
    let startFrac = monthStart / yearAverage
    let startNorm = startFrac - Math.floor( startFrac )

    if ( month === 1 && 0.9 < startNorm ) {
      startNorm = ( restrainValue ) ? 0 :  - ( 1 - startNorm ) // flip to negative
      filteredCount++
      // console.log( fracStart.toString() )
    }
    
    results[0].push( startNorm )

    const monthEnd = Math.floor( new Date( year, month - 1, lastDay, 23, 59, 59, 999 ).getTime() / 1000 ) + timeZoneOffset
    let endFrac = monthEnd / yearAverage
    let endNorm = endFrac - Math.floor( endFrac )

    if ( month === 12 && endNorm < 0.9 ) {
      endNorm = ( restrainValue ) ? 0 : endNorm + 1 // leap one
      filteredCount++
      // console.log( fracEnd.toString() )
    }

    results[1].push( endNorm )

  }

  if ( filteredCount !== 0 ) {
    console.log( `Filtered out count: ${ filteredCount }` )
  }

  return [
    average( results[0] ),
    average( results[1] ),
  ]

}


function secondsInYear() {
  let diffs = []
  for ( let year of yearRange ) {
    const monthStart = Math.floor( new Date( year, 0, 1, 0, 0, 0, 0 ).getTime() / 1000 )
    const monthEnd = Math.floor( new Date( year, 11, 31, 23, 59, 59, 999 ).getTime() / 1000 )
    diffs.push( monthEnd - monthStart )
  }
  const avr = average( diffs )
  console.log( diffs.join( '\n' ) )
  console.log( avr )
}


function range( start, end ) {
  return Array.from(
    Array.from( Array( Math.ceil( end - start + 1 ) ).keys() ),
    x => start + x
  )
}


function average( array ) {
  return array.reduce( ( prev, curr ) => prev + curr ) / array.length
}


// The param `month` sould be in the range 1 to 12 ( Jan. to Dec. )
function getLastDayOfMonth( year, month ) {
  let date = new Date( year, month, 0 )
  return date.getDate()
}

main()
